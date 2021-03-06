
#include "exceptions.h"
#include "utilities.h"
#include "mainwindow.h"
#include "gui/container.h"
#include "gui/control.h"
#include "gui/gui.h"
//Added by qt3to4:
#include <QMouseEvent>
#include <QKeyEvent>

cControl::cControl() {
	visible_ = true;
	needsRealign_ = false;
	disableAlign_ = false;
	x_ = 0;
	y_ = 0;
	width_ = 0;
	height_ = 0;
	parent_ = 0;
	align_ = CA_NONE;
	anchors_ = 0;
	movable_ = false;
	moveHandle_ = false;
	canHaveFocus_ = false;
	wantTabs_ = false;
	tabIndex_ = 0;
	alpha_ = 1.0f;
	ignoreDoubleClicks_ = false;
	wantReturns_ = false;
}

cControl::~cControl() {
	if (Gui->activeWindow() == this) {
		Gui->setActiveWindow(0);
	}
	if (Gui->inputFocus() == this) {
		Gui->setInputFocus(0);
	}
	if (GLWidget && GLWidget->lastMouseMovement() == this) {
		GLWidget->setLastMouseMovement(0);
	}
	if (GLWidget && GLWidget->mouseCapture() == this) {
		GLWidget->setMouseCapture(0);
	}
	if (parent_) {
		parent_->removeControl(this);
	}
}

bool cControl::isVisibleOnScreen() {
	if (!isVisible()) {
		return false;
	} else {
		if (parent_) {
			if (parent_ == Gui) {
				return true; // We are visible here for sure
			}
			return parent_->isVisibleOnScreen();
		} else {
			return false; // We are not owned by the gui if we reach this control path
		}
	}
}

void cControl::setAlign(enControlAlign align) {
	enControlAlign oldalign = align_;
	align_ = align;
	requestAlign();
}

void cControl::setX(int data) {
	int oldx = x_;	
	x_ = data;
	onChangeBounds(oldx, y_, width_, height_);
}

void cControl::setY(int data) {	
	int oldy = y_;	
	y_ = data;
	onChangeBounds(x_, oldy, width_, height_);
}

void cControl::setWidth(int data) {
	int oldwidth = width_;
	width_ = data;
	onChangeBounds(x_, y_, oldwidth, height_);
}

void cControl::setHeight(int data) {
	int oldheight = height_;
	height_ = data;
	onChangeBounds(x_, y_, width_, oldheight);
}

void cControl::setBounds(int x, int y, int width, int height) {
	int oldheight = height_;
	int oldwidth = width_;
	int oldx = x_;
	int oldy = y_;
	x_ = x;
	y_ = y;
	width_ = width;
	height_ = height;
	onChangeBounds(oldx, oldy, oldwidth, oldheight);
}

void cControl::onParentMoved(int oldx, int oldy) {
}

void cControl::setParent(cContainer *data) {
	int oldx = parent_ ? parent_->x() : 0;
	int oldy = parent_ ? parent_->y() : 0;
	parent_ = data;	
	requestAlign();
	//adjustSize();
}

void cControl::onChangeBounds(int oldx, int oldy, int oldwidth, int oldheight) {
	requestAlign();
}

void cControl::requestAlign() {
	if (parent_) {
		parent_->alignControl(this);
	}
}

void cControl::onParentResized(int oldwidth, int oldheight) {
}

void cControl::setVisible(bool data) {
	
	visible_ = data;
	requestAlign();

	if (!data) {
		if (Gui->activeWindow() == this) {
			Gui->setActiveWindow(0);
		}
		if (Gui->inputFocus() == this) {
			Gui->setInputFocus(0);
		}
		if (GLWidget->lastMouseMovement() == this) {
			GLWidget->setLastMouseMovement(0);
		}
		if (GLWidget->mouseCapture() == this) {
			GLWidget->setMouseCapture(0);
		}
	}
}

void cControl::onMouseDown(QMouseEvent *e) {
}

void cControl::onMouseUp(QMouseEvent *e) {
}

cControl *cControl::getControl(int x, int y) {
	if (visible_ && x >= 0 && y >= 0 && x < width_ && y < height_) {
		return this;
	} else {
		return 0;
	}
}

cControl *cControl::getMovableControl() {
	if (isMovable()) {
		return this;
	} else if (parent_) {
		return parent_->getMovableControl();
	} else {
		return 0;
	}
}

void cControl::processDoubleClick(QMouseEvent *e) {
	emit onDoubleClick(this);
}

void cControl::onMouseEnter() {
}

void cControl::onMouseLeave() {
}

void cControl::onBlur(cControl *newFocus) {
}

void cControl::onFocus(cControl *oldFocus) {
}

void cControl::onKeyDown(QKeyEvent *e) {
}

void cControl::onKeyUp(QKeyEvent *e) {
}

void cControl::onClick(QMouseEvent *e) {
}

bool cControl::isContainer() const {
	return false;
}

bool cControl::isWindow() const {
	return false;
}

void cControl::onMouseMotion(int xrel, int yrel, QMouseEvent *e) {
}

void cControl::draw(int xoffset, int yoffset) {
}

QPoint cControl::mapFromGlobal(const QPoint &point) {
	cControl *parent = parent_;
	int x = x_, y = y_;
	while (parent) {
		x += parent->x();
		y += parent->y();
		parent = parent->parent();
	}

    return QPoint(point.x() - x, point.y() - y);
}

cWindow *cControl::getTopWindow() {
	if (parent_) {
		if (parent_->isWindow()) {
			return (cWindow*)parent_;
		} else if (parent_ != Gui) {
			return parent_->getTopWindow();
		}
	}

	return 0;
}

void cControl::setTabIndex(unsigned int data) {
	tabIndex_ = data;
	if (parent_) {
		parent_->sortTabControls();
	}
}

void cControl::processDefinitionElement(QDomElement element) {
	if (element.nodeName() == "bounds") {
		bool ok;
		int x = element.attribute("x").toInt(&ok);
		if (!ok) {
			x = x_;
		}

		int y = element.attribute("y").toInt(&ok);
		if (!ok) {
			y = y_;
		}

		int width = element.attribute("width").toInt(&ok);
		if (!ok) {
			width = width_;
		}

		int height = element.attribute("height").toInt(&ok);
		if (!ok) {
			height = height_;
		}

		setBounds(x, y, width, height);
	} else if (element.nodeName() == "size") {
		bool ok;

		int width = element.attribute("width").toInt(&ok);
		if (!ok) {
			width = width_;
		}

		int height = element.attribute("height").toInt(&ok);
		if (!ok) {
			height = height_;
		}

		setSize(width, height);
	} else if (element.nodeName() == "position") {
		bool ok;
		int x = element.attribute("x").toInt(&ok);
		if (!ok) {
			x = x_;
		}

		int y = element.attribute("y").toInt(&ok);
		if (!ok) {
			y = y_;
		}

		setPosition(x, y);
	} else if (element.nodeName() == "tag") {
		QString name = element.attribute("name");
		QString value = element.attribute("value");

		setTag(name, value);
	}
}

void cControl::processDefinitionAttribute(QString name, QString value) {
	if (name == "visible") {
		setVisible(Utilities::stringToBool(value));
	} else if (name == "align") {
		if (value == "none") {
			setAlign(CA_NONE);
		} else if (value == "left") {
			setAlign(CA_LEFT);
		} else if (value == "top") {
			setAlign(CA_TOP);
		} else if (value == "right") {
			setAlign(CA_RIGHT);
		} else if (value == "bottom") {
			setAlign(CA_BOTTOM);
		} else if (value == "client") {
			setAlign(CA_CLIENT);
		}
	} else if (name == "bottomanchor") {
		setBottomAnchor(Utilities::stringToBool(value));
	} else if (name == "topanchor") {
		setTopAnchor(Utilities::stringToBool(value));
	} else if (name == "leftanchor") {		 
		setLeftAnchor(Utilities::stringToBool(value));
	} else if (name == "rightanchor") {
		setRightAnchor(Utilities::stringToBool(value));
	} else if (name == "movable") {
		setMovable(Utilities::stringToBool(value));
	} else if (name == "movehandle") {
		setMoveHandle(Utilities::stringToBool(value));
	} else if (name == "tabindex") {
		setTabIndex(Utilities::stringToUInt(value));
	} else if (name == "alpha") {
		setAlpha(value.toFloat());
	} else if (name == "name") {
		setObjectName(value);
	} else if (name == "focusable") {
		canHaveFocus_ = Utilities::stringToBool(value);
	}
}

void cControl::setTag(const QString &name, const QString &data) {
	tags.insert(name, data);
}

QString cControl::getTag(const QString &name) {
	QMap<QString, QString>::iterator it = tags.find(name);
	if (it != tags.end()) {
		return it.value();
	} else {
		return QString();
	}
}

bool cControl::hasTag(const QString &name) {
	return tags.find(name) != tags.end();
}

bool cControl::acceptsItemDrop(cDynamicItem *item) {
	return false;
}

void cControl::dropItem(cDynamicItem *item) {
}
