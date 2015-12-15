// Copyright 2014-2015 the openage authors. See copying.md for legal info.

#include <cassert>
#include <iostream>

#include <QGuiApplication>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QQuickRenderControl>
#include <QOffscreenSurface>
#include <QQmlEngine>
#include <QQuickWindow>

#include <QQmlComponent>
#include <QQuickItem>

#include "context_adoption.h"
#include "context_extraction.h"

namespace openage {
namespace gui {

// TODO: separate GUI context and GUI sessions
class RenderControl: public QQuickRenderControl {
public:
	RenderControl(QWindow *w)
			:
				QQuickRenderControl(), //
				window(w) {
	}

	QWindow *renderWindow(QPoint *offset) Q_DECL_OVERRIDE
	{
		if (offset)
			*offset = QPoint(0, 0);

		return window;
	}

	QWindow *window;
};

namespace {
// TODO: separate GUI context and GUI sessions
QOpenGLContext *ctx = nullptr;
RenderControl *renderControl = nullptr;
QQuickWindow *quickWindow = nullptr;
QQmlComponent *qmlComponent = nullptr;
QGuiApplication *app = nullptr;
bool needPolishAndSync = false;
bool needRender = false;
int argc = 1;
char arg[] = "openage";
char *argv = &arg[0];
}

void componentStatusChanged(QQmlComponent::Status status) {
	// TODO: make the load dynamic

	if (QQmlComponent::Ready != status) {
		return;
	}

	QObject* rootObject = qmlComponent->create();
	QQuickItem* rootItem = qobject_cast<QQuickItem*>(rootObject);
	if (!rootItem) {
		return;
	}

	rootItem->setParentItem(quickWindow->contentItem());
	rootItem->setSize(QSizeF(quickWindow->width(), quickWindow->height()));
}

ContextAdoption::ContextAdoption(SDL_Window *window) {
	QVariant handle;
	WId id;

	// TODO: other platforms
	std::tie(handle, id) = extract_native_context(window);

	// TODO: must live and process events in GUI thread
	// TODO: leaks
	app = new QGuiApplication(argc, &argv);

	QWindow *w = QWindow::fromWinId(id);

	// TODO: leaks
	ctx = new QOpenGLContext;
	ctx->setNativeHandle(handle);
	ctx->create();
	assert(ctx->isValid());

	renderControl = new RenderControl(w);

	QObject::connect(renderControl, &QQuickRenderControl::renderRequested, [&] () {
		needRender = true;
	});

	QObject::connect(renderControl, &QQuickRenderControl::sceneChanged, [&] () {
		needPolishAndSync = true;
		needRender = true;
	});

	w->setSurfaceType(QSurface::OpenGLSurface);

	quickWindow = new QQuickWindow(renderControl);
	// TODO: implement resizing
	quickWindow->setGeometry(0, 0, 640, 480);
	quickWindow->setClearBeforeRendering(false);

	if (!ctx->makeCurrent(renderControl->window)) {
		assert(false);
		return;
	}

	renderControl->initialize(ctx);

	assert(ctx->isValid());

	// TODO: separate GUI sessions
	QQmlEngine *qmlEngine = new QQmlEngine;
	if (!qmlEngine->incubationController())
		qmlEngine->setIncubationController(quickWindow->incubationController());

	qmlComponent = new QQmlComponent(qmlEngine);
	QObject::connect(qmlComponent, &QQmlComponent::statusChanged, componentStatusChanged);

	// TODO: make the load dynamic
	const QUrl source = QUrl::fromLocalFile("libopenage/gui/qml/test.qml");
	qmlComponent->loadUrl(source);
}

void ContextAdoption::render() {
	// TODO: maybe unnecessary
	if (!ctx->makeCurrent(renderControl->window)) {
		assert(false);
		return;
	}

	// TODO: process events in GUI thread
	app->processEvents();

	if (needPolishAndSync) {
		needPolishAndSync = false;
		renderControl->polishItems();
		renderControl->sync();
	}

	// TODO: redirect into FBO and render only if renderRequested
	renderControl->render();

	// TODO: find out what it messes up
	quickWindow->resetOpenGLState();
}

} // namespace gui
} // namespace openage
