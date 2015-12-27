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
    mPageNo( 0 ),
    header_logo( "./img/logo1.png", "PNG" ),
    footer_logo( "./img/logo2.png", "PNG" )
{
    ui->setupUi(this);

    QPixmap pixmap( 793, 1123 );
    QPainter painter(&pixmap);
    QRect rc = PreparePage( painter, QRect(0,0,793,1123) );

    if ( test::CURRENT_PARAMS )
    {
        test::CURRENT_PARAMS->Draw( painter, rc );
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
                    pixmap = QPixmap( 793, 1123 );
                    painter.begin( &pixmap );
                    rc = PreparePage( painter, QRect(0,0,793,1123) );
                }
                painter.end();
                painter.begin( &pixmap );
                QFontMetrics m (painter.font());
                rc.setTop( rc.top() + m.height() );
            }
        }

        if ( test::CURRENT_PARAMS->HasResults() )
        {
            painter.end();
            mPages.push_back( pixmap );
            pixmap = QPixmap( 793, 1123 );
            painter.begin( &pixmap );
            rc = PreparePage( painter, QRect(0,0,793,1123) );
            test::CURRENT_PARAMS->DrawResults( painter, rc );
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
        printer.setPageMargins( 20, 20, 5, 10, QPrinter::Millimeter );
        printer.setOutputFileName(file_name);

        QPainter painter(&printer);
        QPoint start (0,0);
        start -= printer.pageRect().topLeft();
        painter.translate( start );
        QRect rc = PreparePage( painter, printer.paperRect() );

        /*for ( int i = 0 ; i < mPages.size(); ++i )
        {
            painter.drawPixmap( rc, mPages[i] );
            if ( i != mPages.size() - 1 )
                printer.newPage();
        }*/        

        if ( test::CURRENT_PARAMS )
        {
            test::CURRENT_PARAMS->Draw( painter, rc );
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
                        rc = PreparePage( painter, printer.paperRect() );
                    }
//                    painter.resetMatrix();
//                    painter.resetTransform();
                    QFontMetrics m (painter.font());
                    rc.setTop( rc.top() + m.height() );
                }
            }
            if ( test::CURRENT_PARAMS->HasResults() )
            {
                printer.newPage();
                rc = PreparePage( painter, printer.paperRect() );
                test::CURRENT_PARAMS->DrawResults( painter, rc );
            }
        }
        painter.end();
    }
}

void Viewer::DrawPage()
{
    ui->mPicture->setPixmap( mPages[mPageNo] );
}

QRect Viewer::PreparePage( QPainter& painter, QRect const& page_rect )
{
    painter.fillRect( page_rect, Qt::white );
    QRect work_area( 76, 76, 698, 1010 );// 698 x 1010 19 = 5мм
    QRect print_area( work_area.left() + 38, work_area.top() + 10, work_area.width() - ( 38 + 19 ), work_area.height() - 19 );

    QRect header_rect( 0, 0, 150, 30 );
    QRect footer_rect( 0, 0, 200, 30 );
    header_rect.setHeight( header_logo.height() * header_rect.width() / header_logo.width() );
    footer_rect.setHeight( footer_logo.height() * footer_rect.width() / footer_logo.width() );

    painter.save();
    QPoint header_point( work_area.right() - header_rect.width(), work_area.top() - header_rect.height() - 2 );
    painter.translate( header_point );
    painter.drawPixmap( header_rect, header_logo );
    painter.restore();

    painter.save();
    QPoint footer_point( work_area.right() - footer_rect.width(), work_area.bottom() + 4 );
    painter.translate( footer_point );
    painter.drawPixmap( footer_rect, footer_logo );
    painter.restore();

    painter.save();
    QFont font = painter.font();
    font.setFamily("Calibri");
    font.setPointSize( 11 );
    painter.setFont( font );
    QFontMetrics m( font );
    QPoint footer_text( work_area.left(), work_area.bottom() + m.height() + 2 );

    painter.drawText( footer_text, "Испытание производилось на стенде испытательном ООО «ПНЕВМАКС»" );
    painter.restore();

    painter.drawRect( work_area );

//    painter.drawRect( print_area );
//    painter.translate( print_area.topLeft() );

    return print_area;
}
