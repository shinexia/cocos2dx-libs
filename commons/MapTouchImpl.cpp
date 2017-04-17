#include "MapTouchImpl.h"
#include "ScaleButton.h"
#include "PolygonMenuItem.h"
#include "GeneralUtils.h"

using namespace cocos2d;


//const int kMapSpritezOrder = -10;

const Vec3 kSmoothFactor = Vec3(0.2f, 0.3f, 0.5f);
const Vec4 kBounceFactor = Vec4(1, 1, 1, 1) * 0.25;
const Vec2 kDampFactor = Vec2(1, 1) * 0.94;
const int _c_rt_width = 3;


MapTouchImpl::MapTouchImpl()
	:_menuItemArray(nullptr)
	,_isTouchInside(nullptr)
	,_touchEnabled(false)
	,_moveEnaled(true)
	,_touchListener(nullptr)
	,_renderTexture(nullptr)
	,_pixelCollisionEnabled(false)
{
	_winSize = Director::getInstance()->getWinSize();

	touch.count = 0;
	touch.isNeedInit = true;
	touch.id1 = -1;
	touch.id2 = -1;
	touch.mode = 0;
	move.mode = MapTouchImpl::Mode::WAITING;
	scale.isAction = false;
	click.selected = nullptr;
}

MapTouchImpl::~MapTouchImpl()
{
	if( _menuItemArray ){
		_menuItemArray->clear();
		delete _menuItemArray;
	}

	CC_SAFE_RELEASE(_touchListener);
	CC_SAFE_RELEASE(_renderTexture);
}

MapTouchImpl* MapTouchImpl::create(const Size& contentSize, float maxscale, float midscale, float minscale, ssize_t buttonCapacity)
{
	MapTouchImpl *pRet = new (std::nothrow)MapTouchImpl();
	if (pRet && pRet->initWithSize(contentSize, maxscale, midscale, minscale, buttonCapacity)){
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}

bool MapTouchImpl::initWithSize(const Size& contentSize, float maxscale, float midscale, float minscale, ssize_t buttonCapacity)
{
	bool bRet = false;
	do {
		CC_BREAK_IF(!Inherited::init());

		this->setContentSize(contentSize);

		scale.maxScaleR = maxscale;
		scale.midScaleR = midscale;

		if( minscale < 0.f ){
			float scaleX = _winSize.width / contentSize.width;
			float scaleY = _winSize.height / contentSize.height;
			scale.minScaleR = scaleX > scaleY ? scaleX : scaleY;
		}
		else{
			scale.minScaleR = minscale;
		}

		if( _menuItemArray ){
			_menuItemArray->clear();
			_menuItemArray->reserve(buttonCapacity);
		}
		else{
			_menuItemArray = new (std::nothrow)Vector<MenuItem*>(buttonCapacity);
		}

		this->setCascadeColorEnabled(true);
		this->setCascadeOpacityEnabled(true);


		//-- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
		// always return true by default.
		_isTouchInside = [](const Vec2& point){return true;};

		this->setTouchEnabled(true);
		this->scheduleUpdate();

		bRet = true;
	}while(0);
	
	return bRet;
}

void MapTouchImpl::setTouchEnabled(bool value)
{
	if(value && _touchEnabled == false){
		if(_touchListener == nullptr){
			auto listener = EventListenerTouchAllAtOnce::create();

			listener->onTouchesBegan = CC_CALLBACK_2(MapTouchImpl::onTouchesBegan, this);
			listener->onTouchesMoved = CC_CALLBACK_2(MapTouchImpl::onTouchesMoved, this);
			listener->onTouchesEnded = CC_CALLBACK_2(MapTouchImpl::onTouchesEnded, this);
			listener->onTouchesCancelled = CC_CALLBACK_2(MapTouchImpl::onTouchesCancelled, this);

			_touchListener = listener;
			_touchListener->retain();
		}

		_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
		_touchEnabled = true;
	}
	else if(value == false && _touchListener != nullptr ){
		if(_touchEnabled){
			_eventDispatcher->removeEventListener(_touchListener);
			_touchEnabled = false;
		}
		_touchListener->release();
		_touchListener = nullptr;

		this->clickCancelled();
		this->touchCancelled();
	}
}

void MapTouchImpl::setPixelCollisionEnabled(bool value)
{
	_pixelCollisionEnabled = value;
	if(value && _renderTexture == nullptr){
		_renderTexture = RenderTexture::create(_c_rt_width, _c_rt_width);
		_renderTexture->setKeepMatrix(true);
		_renderTexture->retain();

		_fullRect.size = Director::getInstance()->getWinSize();
		_fullviewPort.size = Director::getInstance()->getWinSizeInPixels();
	}
}

void MapTouchImpl::onEnter()
{
	if(_touchListener && _touchEnabled == false){
		_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
		_touchEnabled = true;
	}
	Inherited::onEnter();
}

void MapTouchImpl::onExit()
{
	if(_touchEnabled){
		_eventDispatcher->removeEventListener(_touchListener);
		_touchEnabled = false;
	}
	Inherited::onExit();
}

void MapTouchImpl::update(float dt)
{
	if( move.mode == MapTouchImpl::Mode::WAITING && !scale.isAction ){ return; }

	if ( move.mode == MapTouchImpl::Mode::MOVING ){      // common touch.
		// weighted moving average, the average accounts a larger proportion.
		touch.speed = touch.speed * kSmoothFactor.x + touch.delta0 * kSmoothFactor.y + touch.delta * kSmoothFactor.z;

		touch.delta0 = touch.delta; touch.delta.x = touch.delta.y = 0;
	}
	else if ( move.mode == MapTouchImpl::Mode::SLIDING ){ // slide move after touch ended.
		touch.speed.scale( kDampFactor );
	}

	if( scale.isAction ){
		scale.time += dt;
		this->setScale(scale.start + scale.delta * sinf( scale.time / scale.duration * 1.570796));
		if( scale.time >= scale.duration ){
			scale.isAction = false;
		}
	}

	// ( m_obPosition - map.previous ) is to calculate the delta that changed by other actions like scale.Action or proofreadPosition().
	move.start += _position - move.previous + touch.speed;

	this->setPosition( move.start );
	move.previous = move.start;

	if( touch.speed.getLength() < 0.1f ){
		move.mode = MapTouchImpl::Mode::WAITING;         // stop move.
	}

	this->proofreadPosition();
}

void MapTouchImpl::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
	if( !_parent || !_visible ){
		return;
	}

	for (auto iter = touches.rbegin(); iter != touches.rend(); ++iter)
	{
		Touch* pt = *iter;
		if( _isTouchInside( pt->getLocation() ) ){
			this->clickBegan( pt );		//clicks have priority over touch.
			this->touchBegan( pt );
		}
	}
}

void MapTouchImpl::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
	if( !_parent || !_visible || !_moveEnaled){
		return;
	}

	Touch* pt, *pt1 = nullptr, *pt2 = nullptr;
	std::vector<Touch*> list;
	for (auto iter = touches.rbegin(); iter != touches.rend(); ++iter){
		pt = *iter;
		if( _isTouchInside(pt->getLocation()) ){
			if( !_isTouchInside(pt->getPreviousLocation())){
				this->clickBegan( pt );		//clicks have priority over touch.
				this->touchBegan( pt );
			}
			else{
				this->clickMoved( pt );
			}
			list.push_back(pt);
		}
		else if( _isTouchInside(pt->getPreviousLocation()) ){
			this->clickEnded( pt );
			this->touchEnded( pt );
		}
	}

	if(list.size() == 0){
		return;
	}

	if(!touch.isNeedInit){
		if(touch.mode == 0){
			touch.isNeedInit = true;
		}
		else if(touch.mode == 1){
			if(list.size() >= 2 ){
				touch.isNeedInit = true;
			}
		}
		else if(touch.mode == 2){
			if(list.size() < 2){
				touch.isNeedInit = true;
			}
		}

		if(!touch.isNeedInit){
			int count = 0;
			for(auto iter = list.begin(); iter != list.end(); iter++){
				if(touch.id1 == (*iter)->getID()){
					pt1 = *iter;
					++count;
				}
				else if(touch.id2 == (*iter)->getID()){
					pt2 = *iter;
					++count;
				}
			}

			if(count != touch.mode){
				touch.isNeedInit = true;
			}
		}
	}

	if(touch.isNeedInit){
		pt1 = list[0];
		if(list.size() >= 2){
			pt2 = list[1];
		}
	}

	if(list.size() == 1 ){
		this->touchMoved(pt1);
		move.mode = MapTouchImpl::Mode::MOVING;
	}
	else if(list.size() >= 2 ){
		this->touchMoved(pt1, pt2);
		move.mode = MapTouchImpl::Mode::MOVING;
	}

	touch.count = list.size();
}

void MapTouchImpl::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
	if( !_parent || !_visible ){
		return;
	}

	for (auto iter = touches.rbegin(); iter != touches.rend(); ++iter){
		auto pt = *iter;
		if( _isTouchInside(pt->getLocation()) || _isTouchInside(pt->getPreviousLocation()) ){
			this->clickEnded( pt );
			this->touchEnded( pt );
		}
	}
}

void MapTouchImpl::onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event)
{
	this->clickCancelled();
	this->touchCancelled();
}

void MapTouchImpl::touchBegan(Touch* t1)
{
	++touch.count;
	touch.isNeedInit = true;
	move.mode = MapTouchImpl::Mode::WAITING;
	scale.isAction = false;
}

void MapTouchImpl::touchMoved(Touch* t1)
{
	touch.mid = t1->getLocation();

	if( touch.isNeedInit ){
		this->initTouch( t1 );
		touch.isNeedInit = false;
	}

	touch.delta += touch.mid - touch.previous;
	touch.previous = touch.mid;
}

void MapTouchImpl::touchMoved(Touch* t1, Touch* t2)
{
	Vec2 p1 = t1->getLocation();
	Vec2 p2 = t2->getLocation();

	touch.mid = ( p1 + p2 ) / 2.f;

	float dd = p1.getDistance( p2 );

	if( touch.isNeedInit ){   // initialize.
		this->initTouch(t1, t2);
		touch.isNeedInit = false;
	}

	float s = scale.start * (0.2 + dd * 0.8 / touch.distance);

	s = s > scale.maxScaleR ? scale.maxScaleR : s;
	s = s < scale.minScaleR ? scale.minScaleR : s;

	this->setScale( s );

	//to calculate the change by other actions.
	scale.start = s / (0.2 + dd * 0.8 / touch.distance); 

	touch.delta += touch.mid - touch.previous;
	touch.previous = touch.mid;
}

void MapTouchImpl::touchEnded(Touch* t1)
{
	--touch.count;

	if( touch.count == 0 && !touch.isNeedInit){
		move.mode = MapTouchImpl::Mode::SLIDING;
	}
	else{
		float scaleX = this->getScaleX();
		if( touch.count == 1 && scaleX > scale.midScaleR + 0.1f && !touch.isNeedInit){
			scale.duration = 0.42f * ( scaleX - scale.midScaleR ) / ( scale.maxScaleR - scale.midScaleR);
			scale.time = 0;
			scale.start = scaleX;
			scale.delta = scale.midScaleR - scaleX;
			scale.isAction = true;
		}
	}

	touch.isNeedInit = true;
}

void MapTouchImpl::touchCancelled()
{
	touch.count = 0;
	touch.mode = 0;
	move.mode = MapTouchImpl::Mode::WAITING;
	touch.isNeedInit = true;
}

void MapTouchImpl::clickBegan(Touch* t1)
{
	MenuItem* current = this->getItemForTouch(t1);
	if( current != click.selected ){
		if(click.selected ){
			click.selected->unselected();
		}
		click.selected = current;
		if(click.selected){
			click.selected->selected();
		}
	}

	click.moved = false;
}

void MapTouchImpl::clickMoved(Touch* t1)
{
	if(!click.selected){
		return;
	}

	if(!click.moved){
		Vec2 start = t1->getStartLocation();
		Vec2 delta = t1->getLocation() - start;

		if( delta.length() > 5 ){
			click.moved = true;
		}
	}

	if(!click.selected->rect().containsPoint(this->convertToNodeSpace(t1->getLocation()))
		&& click.selected->rect().containsPoint(this->convertToNodeSpace(t1->getPreviousLocation())))
	{
		click.selected->unselected();
		click.selected = nullptr;
	}
}

void MapTouchImpl::clickEnded(Touch* t1)
{
	if(!click.selected ){
		return;
	}

	if(click.selected->rect().containsPoint(this->convertToNodeSpace(t1->getLocation()))
		|| click.selected->rect().containsPoint(this->convertToNodeSpace(t1->getPreviousLocation())))
	{
		auto t = click.selected;

		click.selected->unselected();
		click.selected = nullptr;

		if(!click.moved){
			t->activate();
		}
	}
}

void MapTouchImpl::clickCancelled()
{
	if( !click.selected ) return;

	click.selected->unselected();
	click.selected = nullptr;
}

void MapTouchImpl::initTouch(Touch* t1)
{
	touch.previous = t1->getPreviousLocation();
	touch.speed = touch.delta0 = touch.mid - touch.previous;
	touch.delta.x = touch.delta.y = 0;
	move.previous = move.start = _position;
	touch.id1 = t1->getID();
	touch.id2 = -1;
	touch.mode = 1;
}

void MapTouchImpl::initTouch(Touch* t1, Touch* t2)
{
	Vec2 p1 = t1->getPreviousLocation();
	Vec2 p2 = t2->getPreviousLocation();

	touch.previous = ( p1 + p2 ) / 2.f;
	touch.distance = p1.getDistance( p2 );

	touch.speed = touch.delta0 = touch.mid - touch.previous;
	touch.delta.x = touch.delta.y = 0;

	this->setAnchorWithTouchPoint( touch.mid ); // get move and scale info must after anchor point setted.

	move.previous = move.start = _position;
	scale.start  = this->getScaleX();

	touch.id1 = t1->getID();
	touch.id2 = t2->getID();
	touch.mode = 2;
}

MenuItem* MapTouchImpl::getItemForTouch(Touch* touch)
{
	if(_menuItemArray->empty()){
		return nullptr;
	}

	//Vec2 local = this->convertToNodeSpace();
	for (auto iter = _menuItemArray->begin(); iter != _menuItemArray->end(); ++iter)
	{
		auto button = dynamic_cast<MenuItem*>(*iter);
		if( button && button->isVisible() && button->isEnabled())
		{
			auto pd = dynamic_cast<PolygonMenuItem*>(*iter);
			if(pd && !pd->getPlist().empty()){
				if(GeneralUtils::newPointIsInPolygon(button->convertToNodeSpace(touch->getLocation()),pd->getPlist())){
					return button;
				}else{
					continue;
				}
			}else{
				Vec2 local = button->convertToNodeSpace(touch->getLocation());
				Rect r = button->rect();
				r.origin = Vec2::ZERO;
				if(r.containsPoint( local ) ){
					if(!_pixelCollisionEnabled){
						return button;
					}
					else if(pixelCollisionCheck(button, touch->getLocation())){
						return button;
					}
				}
			}
		}
	}
	return nullptr;
}

bool MapTouchImpl::pixelCollisionCheck(Node* item, const Vec2& position)
{
	_renderTexture->setVirtualViewport(
		Vec2(position.x - _c_rt_width * 0.5, position.y - _c_rt_width * 0.5),
		_fullRect,
		_fullviewPort
		);
	
	_renderTexture->beginWithClear( 0, 0, 0, 0);

    item->visit();
  
    _renderTexture->end();

	Director::getInstance()->getRunningScene()->visit();

	Image* img = _renderTexture->newImage();
    unsigned int *pixel = (unsigned int *)img->getData();

    for(unsigned int i=0; i<_c_rt_width * _c_rt_width; i++) {
		int alpha = (pixel[i] >> 24) & 0xff;
		if(alpha > 50){
			img->release();
			return true;
		}
    }

	img->release();
	return false;
}

void MapTouchImpl::setAnchorWithTouchPoint(const Vec2& point)
{
	Vec2 local = _parent->convertToNodeSpace( point );
	
	Vec2 newAnchor;
	newAnchor.x = (( local.x - _position.x ) /  _scaleX + _anchorPointInPoints.x ) / _contentSize.width;
	newAnchor.y = (( local.y - _position.y ) /  _scaleY + _anchorPointInPoints.y ) / _contentSize.height;

	this->setAnchorPoint( newAnchor );
	this->setPosition( local );
}

void MapTouchImpl::proofreadPosition()
{
	Vec2 anchorPoint = this->getAnchorPointInPoints();

	Vec2 contentSizeInVec2 = Vec2(_contentSize.width, _contentSize.height);
	Vec2 sl = Vec2(_scaleX, _scaleY);

	anchorPoint.scale( sl );
	contentSizeInVec2.scale( sl );

	// convert anchor point to the bottom left position.
	Vec2 tmp = _position - anchorPoint;

	if( tmp.x > 0 ){					// left.
		tmp.x = 0;
		if( touch.speed.x > 0 )
			touch.speed.x = ( move.mode == MapTouchImpl::Mode::SLIDING ) ? -touch.speed.x * kBounceFactor.x : 0;
	}
	if( tmp.y > 0 ){					// bottom.
		tmp.y = 0;
		if( touch.speed.y > 0 )
			touch.speed.y = ( move.mode == MapTouchImpl::Mode::SLIDING ) ? -touch.speed.y * kBounceFactor.y : 0;
	}

	// convert to the top right.
	tmp = tmp + contentSizeInVec2;

	if( tmp.x < _winSize.width ){		// right.
		tmp.x = _winSize.width;
		if( touch.speed.x < 0 )
			touch.speed.x = ( move.mode == MapTouchImpl::Mode::SLIDING ) ? -touch.speed.x * kBounceFactor.z : 0;
	}
	if( tmp.y < _winSize.height ){		// top.
		tmp.y = _winSize.height;
		if( touch.speed.y < 0 )
			touch.speed.y = ( move.mode == MapTouchImpl::Mode::SLIDING ) ? -touch.speed.y * kBounceFactor.w : 0;
	}

	// back to the anchor point.
	tmp = tmp - contentSizeInVec2 + anchorPoint;

	this->setPosition( tmp );
}

void MapTouchImpl::addMenuItem(MenuItem* button)
{
	this->addChild( button );
	_menuItemArray->pushBack( button );
}

void MapTouchImpl::addMenuItem(MenuItem* button, int zdr)
{
	this->addChild( button, zdr);
	_menuItemArray->pushBack( button );
}

void MapTouchImpl::addMenuItem(MenuItem* button, int zdr, int tag)
{
	this->addChild( button, zdr, tag);
	_menuItemArray->pushBack( button );
}

void MapTouchImpl::removeMenuItem(MenuItem* button, bool cleanup)
{
	this->removeChild( button, cleanup );
	_menuItemArray->eraseObject(button);
}