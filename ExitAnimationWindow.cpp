// ExitAnimationWindow.cpp
#include "ExitAnimationWindow.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QScreen>

ExitAnimationWindow::ExitAnimationWindow(QWidget* parent)
    : QWidget(parent), gifLabel(new QLabel(this)), exitMovie(nullptr)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    setupAnimation();
}

ExitAnimationWindow::~ExitAnimationWindow()
{
    if (exitMovie) exitMovie->stop();
}

void ExitAnimationWindow::setupAnimation()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    gifLabel->setAlignment(Qt::AlignCenter);
    gifLabel->setScaledContents(false);
    layout->addWidget(gifLabel);

    exitMovie = new QMovie(":/gifs/special/bowbig0.gif");

    connect(exitMovie, &QMovie::frameChanged, [this](int frame) {
        if (frame >= 0) {
            QImage img = exitMovie->currentImage();
            img = img.scaled(gifLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            gifLabel->setPixmap(QPixmap::fromImage(img));
        }
        });

    connect(exitMovie, &QMovie::finished, this, &QWidget::close);
}

void ExitAnimationWindow::playExitAnimation(QWidget* parent)
{
    ExitAnimationWindow* window = new ExitAnimationWindow(nullptr);

    // 设置窗口尺寸（根据实际GIF尺寸调整）
    const int gifWidth = 1800;
    const int gifHeight = 1011.6;
    window->setFixedSize(gifWidth, gifHeight);
    // 居中显示
    QScreen* screen = QApplication::primaryScreen();
    QRect screenRect = screen->geometry();
    // 左对齐 + 下边缘对齐
    int x = 0;
    int y = screenRect.height() - gifHeight - 45;
    window->move(x, y);

    window->show();
    window->exitMovie->start();
    QTimer::singleShot(2500, window, &QWidget::close);  // 添加这一行
    // 创建事件循环等待动画完成
    QEventLoop loop;
    QObject::connect(window, &ExitAnimationWindow::destroyed, &loop, &QEventLoop::quit);
    loop.exec();

}
