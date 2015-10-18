#include "viewer.h"
#include "ui_viewer.h"
#include <test_case/test_params.h>
#include <test_case/test.h>
#include <QFileDialog>
#include "settings/settings.h"
#include <QPrintEngine>

Viewer::Viewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Viewer),
    mPageNo( 0 )
{
    ui->setupUi(this);

    QPixmap pixmap( 680, 1085 );
    QPainter painter(&pixmap);
    QRect rc(0,0,680,1085);
    painter.fillRect( rc, Qt::white );

    if ( test::CURRENT_PARAMS )
    {
        foreach (test::Test* test, test::CURRENT_PARAMS->TestCase())
        {
            test->ResetDrawLine();

            bool draw = false;
            while( !draw )
            {
                draw = test->Draw( painter, rc );
                if ( !draw )
                {
                    painter.end();
                    mPages.push_back( pixmap );
                    pixmap = QPixmap( 680, 1085 );
                    rc = QRect( 0, 0, 680, 1085 );
                    painter.begin( &pixmap );
                    painter.fillRect( rc, Qt::white );
                }
                painter.end();
                painter.begin( &pixmap );
                QFontMetrics m (painter.font());
                rc.setTop( rc.top() + m.height() );
            }
        }
    }
    painter.end();
    mPages.push_back( pixmap );

    DrawPage();
}

Viewer::~Viewer()
{
    delete ui;
}
void Viewer::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent( e );
    emit closed();
}
void Viewer::on_pushButton_clicked()
{
    if ( mPageNo > 0 )
        --mPageNo;
    DrawPage();
}

void Viewer::on_pushButton_2_clicked()
{
    if ( mPageNo < ( mPages.size() - 1 ) )
        ++mPageNo;
    DrawPage();
}

void Viewer::on_Save_clicked()
{
    QString file_name;
    QFileDialog dlg;
    dlg.setFileMode( QFileDialog::AnyFile );
    dlg.setDirectory( app::Settings::Instance().TestPath() );
    dlg.setNameFilter( "Результаты испытаний (*.res )" );
    dlg.setAcceptMode( QFileDialog::AcceptSave );
    dlg.setViewMode( QFileDialog::Detail );
    if ( dlg.exec() )
        file_name = dlg.selectedFiles().front();
    if ( !file_name.isEmpty() )
    {
        if ( !file_name.endsWith(".res", Qt::CaseInsensitive) )
            file_name += ".res";
        test::DataToFile( file_name, *test::CURRENT_PARAMS );
    }
}

void Viewer::on_SavePDF_clicked()
{
    QString file_name;
    QFileDialog dlg;
    dlg.setFileMode( QFileDialog::AnyFile );
    dlg.setDirectory( app::Settings::Instance().TestPath() );
    dlg.setNameFilter( "Результаты PDF (*.pdf )" );
    dlg.setAcceptMode( QFileDialog::AcceptSave );
    dlg.setViewMode( QFileDialog::Detail );
    if ( dlg.exec() )
        file_name = dlg.selectedFiles().front();
    if ( !file_name.isEmpty() )
    {
        if ( !file_name.endsWith(".pdf", Qt::CaseInsensitive) )
            file_name += ".pdf";
        QPrinter printer;
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOrientation( QPrinter::Portrait );
        printer.setPaperSize( QPrinter::A4 );
        printer.setPageMargins( 20, 5, 10, 5, QPrinter::Millimeter );
        printer.setOutputFileName(file_name);

        QPainter painter(&printer);
        QRect rc( 0, 0, printer.pageRect().width(), printer.pageRect().height() );

        /*for ( int i = 0 ; i < mPages.size(); ++i )
        {
            painter.drawPixmap( rc, mPages[i] );
            if ( i != mPages.size() - 1 )
                printer.newPage();
        }*/
        if ( test::CURRENT_PARAMS )
        {
            foreach (test::Test* test, test::CURRENT_PARAMS->TestCase())
            {
                test->ResetDrawLine();

                bool draw = false;
                while( !draw )
                {
                    draw = test->Draw( painter, rc );
                    if ( !draw )
                    {
                        printer.newPage();
                        rc = QRect( 0, 0, printer.pageRect().width(), printer.pageRect().height() );
                    }
                    painter.resetMatrix();
                    painter.resetTransform();
                    QFontMetrics m (painter.font());
                    rc.setTop( rc.top() + m.height() );
                }
            }
        }
        painter.end();
    }
}

void Viewer::DrawPage()
{
    QPixmap pixmap( 793, 1123 );
    QPainter painter(&pixmap);
    QRect rc(0,0,793,1123);
    QRect rc2(76,19,680,1085);
    painter.fillRect( rc, Qt::white );
    painter.drawPixmap( rc2, mPages[mPageNo] );
    painter.end();
    ui->mPicture->setPixmap( pixmap );
}
