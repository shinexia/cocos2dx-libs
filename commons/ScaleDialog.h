/**
* created by shine xia <http://www.xgh.io>
*/


#ifndef __SCALE_DIALOG_H__
#define __SCALE_DIALOG_H__


#include "PlaneDialog.h"

class ScaleDialog : public PlaneDialog
{
private:
	typedef PlaneDialog Inherited;

public:
	virtual void show();
	virtual void dismiss();
};

#endif //__SCALE_DIALOG_H__
