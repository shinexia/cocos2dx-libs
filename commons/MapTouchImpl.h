/**
* created by shine xia <http://www.xgh.io>
*
* 这个是一个可以拖动、放大、缩小的地图实现，优势
* 1. 支持多点触控
* 2. 做了平滑处理
* 3. 可以设置排除区域
*/


#ifndef __MAP_TOUCH_IMPL_H__
#define __MAP_TOUCH_IMPL_H__


#include "cocos2d.h"

class MapTouchImpl : public cocos2d::Node
{
private:
	typedef cocos2d::Node Inherited;

	typedef std::function<bool(const cocos2d::Vec2& point)> __TouchInsideHandler;

	enum class Mode{
		WAITING,MOVING,SLIDING
	};

public:
	static  MapTouchImpl* create(const cocos2d::Size& contentSize, float maxscale = 1.618, float midscale = 1.0, float minscale = -1.0, ssize_t buttonCapacity = 29);
	bool initWithSize(const cocos2d::Size& contentSize, float maxscale, float midscale, float minscale, ssize_t buttonCapacity);

	bool  isTouchEnabled(){return _touchEnabled;};
	void  setTouchEnabled(bool value);
	void  setMoveEnabled(bool value){_moveEnaled = value;};

	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void update(float dt) override;

	virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
	virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
	virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
	virtual void onTouchesCancelled(const std::vector<cocos2d::Touch*>&touches, cocos2d::Event *unused_event);

	void setTouchInsideHandler(const __TouchInsideHandler& newHandler){_isTouchInside = newHandler;}

	void setAnchorWithTouchPoint(const cocos2d::Vec2& point);

	void setPixelCollisionEnabled(bool );

	void addMenuItem(cocos2d::MenuItem* button);
	void addMenuItem(cocos2d::MenuItem* button, int zdr);
	void addMenuItem(cocos2d::MenuItem* button, int zdr, int tag);

	void removeMenuItem(cocos2d::MenuItem* button, bool cleanup = true);

	cocos2d::Vector<cocos2d::MenuItem*>* getMenuItemArray(){return _menuItemArray;}

	void proofreadPosition();


CC_CONSTRUCTOR_ACCESS:
	MapTouchImpl();
	~MapTouchImpl();


protected:
	void  touchBegan(cocos2d::Touch* t1);
	void  touchMoved(cocos2d::Touch* t1);
	void  touchMoved(cocos2d::Touch* t1, cocos2d::Touch* t2);
	void  touchEnded(cocos2d::Touch* t1);
	void  touchCancelled();

	void  clickBegan(cocos2d::Touch* t1);
	void  clickMoved(cocos2d::Touch* t1);
	void  clickEnded(cocos2d::Touch* t1);
	void  clickCancelled();

	void  initTouch(cocos2d::Touch* t1);
	void  initTouch(cocos2d::Touch* t1, cocos2d::Touch* t2);

	cocos2d::MenuItem* getItemForTouch(cocos2d::Touch* touch);

	bool pixelCollisionCheck(cocos2d::Node* , const cocos2d::Vec2& );


protected:
	cocos2d::Vector<cocos2d::MenuItem*>* _menuItemArray;

	__TouchInsideHandler   _isTouchInside;

	cocos2d::RenderTexture *_renderTexture;
	cocos2d::Rect          _fullRect;
	cocos2d::Rect          _fullviewPort;
	bool                   _pixelCollisionEnabled;

	struct {
		cocos2d::Vec2   mid;		// middle of the first two fingers.
		cocos2d::Vec2   previous;	// previous mid.
		cocos2d::Vec2   delta;
		cocos2d::Vec2   delta0;		// previous delta0.
		cocos2d::Vec2   speed;
		float  distance;			// distance between the first two fingers.
		int    count;				// number of touches inside.
		int    mode; // 0 init, 1 single, 2 double;
		bool   isNeedInit;
		int id1;
		int id2;
	} touch;

	bool _touchEnabled;
	bool _moveEnaled;
	cocos2d::EventListener*  _touchListener;

	struct {
		cocos2d::Vec2   start;
		cocos2d::Vec2   previous;
		MapTouchImpl::Mode   mode;
	} move;

	struct {
		float  time;
		float  duration;
		float  start;
		float  delta;
		float  maxScaleR;
		float  midScaleR;
		float  minScaleR;
		bool   isAction;	// do scale back action.
	} scale;

	struct {
		cocos2d::MenuItem*  selected;
		bool       moved;
	} click;

	cocos2d::Size    _winSize;
};

#endif //__MAP_TOUCH_IMPL_H__
