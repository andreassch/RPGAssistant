#include "AboutWidget.h"
#include "ui_AboutWidget.h"

#if (defined(RPG_SYSTEM_TDE_AVENTURIA) && defined(EDITION_SCRIPTORIUM_AVENTURIS))
#include "TDEScriptoriumAventuris.h"
#endif


AboutWidget::AboutWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::AboutWidget)
{
    m_ui->setupUi(this);
    setupUi();
}

AboutWidget::~AboutWidget()
{
    delete m_ui;
}

void AboutWidget::setupUi()
{
    // Set up App logo and copyright.
    m_ui->labelAppCopyright->setText(tr(
        "Roleplaying Game Assistant\n"
        "Version for %1\n"
        "© 2020 Andreas Schneider\n"
        "andreas_schn@web.de"
        ).arg(rpgSystemName()));

    // Set up system-specific text and logo.
#ifdef RPG_SYSTEM_TDE_AVENTURIA
#ifdef EDITION_SCRIPTORIUM_AVENTURIS
    setMinimumSize(500, 750);
#ifdef ANDROID
    m_ui->labelLibraryCopyright->setFont(QFont("Sans",9));
    m_ui->labelSystemCopyright->setFont(QFont("Sans",9)); // decrease font size to fit everything on screen
    m_ui->labelSystemCopyright->setMinimumSize(QSize(500,500));
#endif
    m_ui->labelSystemCopyright->setText(TDEScriptoriumAventurisCopyright());
    m_ui->labelSystemLogo->setPixmap(TDEScriptoriumAventurisLogo());
#else // ! EDITION_SCRIPTORIUM_AVENTURIS
    m_ui->labelSystemCopyright->setText(tr(
        "The Dark Eye, its respective logos, as well as Aventuria, Dere, Myranor, Riesland, Tharun and Uthuria, and their respective logos, are trademarks of Ulisses Spiele GmbH, Waldems."
        ));
    // Official German translation: DAS SCHWARZE AUGE, AVENTURIEN, DERE, MYRANOR, THARUN, UTHURIA, RIESLAND und THE DARK EYE sind eingetragene Marken der Ulisses Spiele GmbH, Waldems.
#endif // ! EDITION_SCRIPTORIUM_AVENTURIS
#endif // RPG_SYSTEM_TDE_AVENTURIA

    return;
}

QString AboutWidget::rpgSystemName()
{
#ifdef RPG_SYSTEM_TDE_AVENTURIA
    return tr("The Dark Eye/Aventuria");
#else
    return tr("general role playing system");
#endif
}
