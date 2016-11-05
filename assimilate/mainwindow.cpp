#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstring>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  ui(new Ui::MainWindow) {
	this->xPressed = 0;
	this->yPressed = 0;
	this->numCounts = 0;
	this->stepCounts = 0;
	memset(checkBoard, 0, sizeof checkBoard);
	/*
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
				checkBoard[i][j] = 0;
		*/
	this->setMaximumHeight(600);
	this->setMinimumHeight(600);
	this->setMinimumSize(600, 600);
	this->setMaximumSize(600, 600);
	ui->setupUi(this);
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::paintEvent(QPaintEvent *) {
	QPainter painter(this);
	QPen pen;
	pen.setColor(Qt::blue);
	painter.setPen(pen);

	// Draw checkboard
	for (int i = 20; i < 600; i += 80) {
		QPoint u(20, i);
		QPoint v(580, i);
		painter.drawLine(u, v);
	}
	for (int i = 20; i < 600; i += 80) {
		QPoint u(i, 20);
		QPoint v(i, 580);
		painter.drawLine(u, v);
	}

	// Draw chess pieces
	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			if (this->checkBoard[i][j]) {
				brush.setColor(checkBoard[i][j] == -1 ? Qt::white : Qt::black);
				painter.setBrush(brush);
				painter.drawEllipse(25 + 80 * i, 25 + 80 * j, 70, 70);
			}
}

void MainWindow::mousePressEvent(QMouseEvent *mouseEvent) {
	int x = float(mouseEvent->x());
	int y = float(mouseEvent->y());
	if (x < 20 || x > 580 || y < 20 || y > 580 || (x - 20) % 80 == 0 || (y - 20) % 80 == 0)
		return ;
	x = (x - 20) / 80, y = (y - 20) / 80;
	int stand = (this->stepCounts & 1) ? -1 : 1;
	this->checkBoard[x][y] = stand;
	this->update();

	this->stepCounts++;
	this->numCounts++;
	if (this->numCounts == 49)
		checkWin();
}

void MainWindow::checkWin() {
	int counts = 0;
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			counts += this->checkBoard[i][j];
	winShow(counts < 0 ? -1 : 1);
}

void MainWindow::winShow(int stand) {
	QString info;
	info = stand == 1 ? "Black Win!" : "White Win!";
	if (QMessageBox::information(NULL, "Game Over!", info, "Again", "Exit"))
		close();
	clear();
}

void MainWindow::clear() {
	this->numCounts = this->stepCounts = 0;
	memset(this->checkBoard, 0, sizeof this->checkBoard);
}


