#include "QT_OpenCV.h"
#include"ui_QT_OpenCV.h"
#include"check.h"
#include<qmessagebox.h>
#include<qdebug.h>
#pragma execution_character_set("utf-8")
int c = 0;
QT_OpenCV::QT_OpenCV(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	this->setWindowTitle("����ʶ����� ");
	connect(&serial, &QSerialPort::readyRead, this, &QT_OpenCV::serialPort_readyRead);
	ui.cbb_com->clear();
	//���п�ʼ���ҿ��ô���
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		ui.cbb_com->addItem(info.portName());  
		qDebug() << "�����������";
		ui.tb_rx_buf->append("�����������");
	}
	ui.cbb_baud->insertItem(0, "9600");
	ui.cbb_baud->insertItem(1, "115200");
	ui.cbb_baud->insertItem(2, "384000");
	ui.cbb_baud->insertItem(3, "�Զ���");

	//��ʼ����ťʧ��
	ui.pushButton_5->setDisabled(true);    //���ư�ť������
	ui.pushButton_7->setDisabled(true);
	ui.pushButton_8->setDisabled(true);
	ui.btn_send->setDisabled(true);
	ui.cb_send_enter->setDisabled(true);
	ui.te_tx_buf->setDisabled(true);
	//������ʾ���������Ϊ10��
	ui.tb_rx_buf->document()->setMaximumBlockCount(10);
}

//�򿪴��ڰ�ť
void QT_OpenCV::on_btn_uart_Ctrl_clicked()
{
	if (ui.btn_uart_Ctrl->text() == "�򿪴���")   //��ʼ״̬�����ô��ڲ���
	{
		serial.setPortName(ui.cbb_com->currentText());     //���ô��ںš�
		serial.setBaudRate(ui.cbb_baud->currentText().toInt());    //���ò�����
		serial.setDataBits(QSerialPort::Data8);     //���ô�������λ8
		serial.setParity(QSerialPort::NoParity);    //��У��λ
		serial.setStopBits(QSerialPort::OneStop);   //1λֹͣλ
		serial.setFlowControl(QSerialPort::NoFlowControl);
		//�򿪴���
		if (!serial.open(QIODevice::ReadWrite))
		{
			QMessageBox::critical(NULL, "��ʾ", "���ڴ�ʧ��");
			return;
		}
		ui.tb_rx_buf->append("���ڴ򿪳ɹ�");
		ui.btn_uart_Ctrl->setText("�رմ���");

		ui.pushButton_5->setEnabled(true);    //���ư�ť����
		ui.pushButton_7->setEnabled(true);
		ui.pushButton_8->setEnabled(true);
		ui.btn_send->setEnabled(true);
		ui.te_tx_buf->setEnabled(true);
		ui.cb_send_enter->setEnabled(true);
	}
	else
	{
		//�رմ���
		serial.close();
		ui.btn_uart_Ctrl->setText("�򿪴���");
		ui.tb_rx_buf->append("���ڹرճɹ�");

		ui.pushButton_5->setDisabled(true);    //���ư�ť������
		ui.pushButton_7->setDisabled(true);
		ui.pushButton_8->setDisabled(true);
		ui.btn_send->setDisabled(true);
		ui.cb_send_enter->setDisabled(true);
		ui.te_tx_buf->setDisabled(true);
	}
}

void QT_OpenCV::on_pushButton_5_clicked()
{
	QPalette colr;
	if (ui.pushButton_5->text() == "����")
	{
		colr.setColor(QPalette::WindowText, Qt::blue);
		ui.label_5->setPalette(colr);
		ui.pushButton_5->setText("ֹͣ");
		serial.write("A1\n");     //������ͨ�����ڷ��ͳ�ȥ
		ui.tb_rx_buf->append("����ָ��A1��ϵͳ�����ɹ�");
	}
	else
	{
		colr.setColor(QPalette::WindowText, Qt::black);
		ui.label_5->setPalette(colr);        //���ñ�ǩ��ɫ��ɫ
		ui.pushButton_5->setText("����");
		serial.write("A2\n");     //���ڷ���A2
		ui.tb_rx_buf->append("����ָ��A2,ϵͳֹͣ");
	}
}


void QT_OpenCV::on_pushButton_7_clicked()
{
	if (ui.pushButton_7->text() == "��ת")
	{
		serial.write("A3\n");     //������ͨ�����ڷ��ͳ�ȥ
		ui.tb_rx_buf->append("����ָ��A3,���������ת");
	}
}

void QT_OpenCV::on_pushButton_8_clicked()
{
	if (ui.pushButton_8->text() == "��ת")
	{
		serial.write("A4\n");     //������ͨ�����ڷ��ͳ�ȥ
		ui.tb_rx_buf->append("����ָ��A4,���������ת");
	}
}

//�������ݽ��ղ���ʾ
void QT_OpenCV::serialPort_readyRead()
{
	QByteArray rx_buf = serial.readAll();   //��ȡ���ڽ��յ�����
	/*
	if(rx_buf.endsWith("\r\n")) //�жϽ�������Ƿ��ǻس����У���������ɱ�־
	{

	}
	*/
	QString rx_buf_tmp = QString::fromLocal8Bit(rx_buf);    //ת��Ϊ���ĸ�ʽ
	/*
	QTĬ�ϵı�����unicode��������ʾ���ĵ�
	windowsĬ��ʹ�ã�GBK/GB2312/GB18030��
	ʹ��fromLocal8Bit()������ʵ���˴ӱ����ַ���GB��Unicode��ת�������ڴ�������ʾ���������
	*/
	qDebug() << rx_buf_tmp;         //����̨���

	ui.tb_rx_buf->append(rx_buf_tmp);

	rx_buf_tmp.clear();
	rx_buf.clear();
}

void QT_OpenCV::on_btn_serch_clicked()
{
	ui.cbb_com->clear();
	//���п�ʼ���ҿ��ô���
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		ui.cbb_com->addItem(info.portName());  //���ں������˵�������һ����Ŀ��Ϊ���ں�COM4
		ui.tb_rx_buf->append("�����������");
	}
}

//�Զ��岨����
void QT_OpenCV::on_cbb_baud_currentIndexChanged(const QString &arg1)
{
	if (ui.cbb_baud->currentIndex() == 3)
	{
		ui.cbb_baud->setItemText(3, ""); //�����Զ��岨����ʱ����������Ϊ�գ�׼����������
		ui.cbb_baud->setEditable(true);
	}
	else
	{
		ui.cbb_baud->setItemText(3, "�Զ���"); //�����Զ��岨����ʱ����������Ϊ�գ�׼����������
		ui.cbb_baud->setEditable(false);
	}
	serial.setBaudRate(ui.cbb_baud->currentText().toInt());    //��ʹ�򿪴��ں���Ȼ�������ò�����
}

//������մ��ڰ�ť
void QT_OpenCV::on_btn_clear_clicked()
{
	ui.tb_rx_buf->clear();
}

//���Ͱ�ť������
void QT_OpenCV::on_btn_send_clicked()
{
	//��ȡ�������������ݲ�ת��ΪUTF8��ʽ
	QByteArray tx_buf = ui.te_tx_buf->toPlainText().toUtf8();

	if (send_enter_flag == true)
		tx_buf += "\n";

	serial.write(tx_buf);     //������ͨ�����ڷ��ͳ�ȥ
	ui.tb_rx_buf->append("ָ��ͳɹ�");
	ui.te_tx_buf->clear();
}
//�Ƿ�������
void QT_OpenCV::on_cb_send_enter_clicked()
{
	if (ui.cb_send_enter->isChecked())
	{
		send_enter_flag = true;
		ui.tb_rx_buf->append("��������");
	}
	else
	{
		send_enter_flag = false;
		ui.tb_rx_buf->append("����������");
	}
}

void QT_OpenCV::on_OpenCameraBtn_clicked()  //������ͷ
{
	VideoCapture capture(0);
	ui.tb_rx_buf->append("�ϰ���ʶ��");
	while (true)
	{
		capture.read(img);
		newpoints = findcolor(img);
		//drawcri(newpoints,mycolorvalues);
		imshow("video", img);
		waitKey(1);
		if (a==1)
		{
			break;
		}
		if (b == 1)
		{
			ui.tb_rx_buf->setText("ǰ�����ϰ���");
		}
	}
}
void QT_OpenCV::on_pushButton_clicked()
{
	Mat imag;
	VideoCapture capture(0);
	ui.tb_rx_buf->append("����ʶ��");
	CascadeClassifier facecascade;
	facecascade.load("haarcascade_frontalface_default.xml");

	if (facecascade.empty())
	{
		cout << "Xml file not loaded" << endl;
	}
	vector<Rect> faces;
	while (true)
	{
		capture.read(imag);
		facecascade.detectMultiScale(imag, faces, 1.1, 10);

		for (int i = 0; i < faces.size(); i++)
		{
			c = 1;
			rectangle(imag, faces[i].tl(), faces[i].br(), Scalar(255, 0, 255), 5);
		}
		imshow("Image",imag);
		waitKey(1);
		if (a == 1)
		{
			break;
		}	
		if (c == 1)
	    {
		ui.tb_rx_buf->setText("ǰ������");
	    }
	}
}
void QT_OpenCV::on_CloseCameraBtn_clicked()  //�ر�����ͷ
{
	ui.tb_rx_buf->append("�ر�����ͷ");
	a = 1;
	/*VideoCapture capture;*/
	VideoCapture capture;
	capture.release();
}

void QT_OpenCV::on_HSV_clicked()  //HSVѡȡ
{
	    ui.tb_rx_buf->append("HSVѡȡ");
		VideoCapture cap(0);
		Mat img;
		Mat imgHSV, mask, imgcolor;

		int hmin = 0, smin = 0, vmin = 0;
		int hmax = 179, smax = 255, vmax = 255;

		namedWindow("Trackbars", (640, 300));
		createTrackbar("ɫ����Сֵ", "Trackbars", &hmin, 179);
		createTrackbar("ɫ�����ֵ", "Trackbars", &hmax, 179);
		createTrackbar("���Ͷ���Сֵ", "Trackbars", &smin, 255);
		createTrackbar("���Ͷ����ֵ", "Trackbars", &smax, 255);
		createTrackbar("������Сֵ", "Trackbars", &vmin, 255);
		createTrackbar("�������ֵ", "Trackbars", &vmax, 255);

		while (true)
		{
			cap.read(img);
			cvtColor(img, imgHSV, COLOR_BGR2HSV);
			Scalar lower(hmin, smin, vmin);
			Scalar upper(hmax, smax, vmax);
			inRange(imgHSV, lower, upper, mask);
			cout << hmin << "," << smin << "," << vmin << "," << hmax << "," << smax << "," << vmax << endl;
			imshow("img", img);
			//imshow("imghsv", imgHSV);
			imshow("imgmask", mask);
			waitKey(1);
		}
}

void QT_OpenCV::on_actionRedo_triggered()
{
	ui.tb_rx_buf->redo();
}

void QT_OpenCV::on_actionUndo_triggered()
{
	ui.tb_rx_buf->undo();
}