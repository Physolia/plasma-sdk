/*
  Copyright (c) 2009 Riccardo Iaconelli <riccardo@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KParts/MainWindow>
#include <KLibLoader>
#include <kservice.h>

class QModelIndex;

namespace Ui
{
}

class QDockWidget;
class QStringList;


class EditPage;
class PackageModel;
class StartPage;
class Sidebar;
class TimeLine;
class MetaDataEditor;

// our own previewer
class Previewer;
class DocBrowser;
class Publisher;

namespace KTextEditor
{
    class Document;
    class View;
} // namespace KTextEditor

class MainWindow : public KParts::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QStringList recentProjects();

public Q_SLOTS:
    void quit();

    void changeTab(const QModelIndex &);
    void loadProject(const QString &name, const QString &type);

    void loadRequiredEditor(const KService::List offers, KUrl target);
    void loadMetaDataEditor(KUrl target);
    void saveEditorData();
    void saveAndRefresh();
    void refreshNotes();
    // refreshes editor, killing all local changes
    void editorDestructiveRefresh();

signals:
    void newSavePointClicked();
    void refreshRequested();

private:

    // QMainWindow takes control of and DELETES the previous centralWidget
    // whenever a new one is set - this is bad because we want to preserve
    // the state of the previous centralWidget for when it becomes active again.
    // This class is a workaround - we set an instance as the permanent
    // centralWidget, and use it to do graceful, non-destructive widget-switching.
    class CentralContainer : public QWidget
    {
    public:
        enum SwitchMode {
            Preserve,
            DeleteAfter
        };
        CentralContainer(QWidget* parent);
        void switchTo(QWidget* newWidget, SwitchMode mode = Preserve);
    private:
        SwitchMode m_curMode;
        QLayout *m_layout;
        QWidget *m_curWidget;
    };

    enum WorkflowTabs { StartPageTab = 0,
                        EditTab,
                        SavePoint,
                        PublishTab,
                        DocsTab,
                        PreviewTab
                      };

    void createMenus();
    void createDockWidgets();
    void setupTextEditor(KTextEditor::Document *editorPart, KTextEditor::View *view);
    void loadNotesEditor(QDockWidget *container);
    Previewer* createPreviewerFor(const QString& projectType);

    StartPage *m_startPage;
    Sidebar *m_sidebar;
    TimeLine    *m_timeLine;
    Previewer *m_previewerWidget;
    MetaDataEditor *m_metaEditor;
    Publisher *m_publisher;
    DocBrowser *m_browser;

    QDockWidget *m_projectNotesWidget;

    QDockWidget *m_editWidget;
    EditPage *m_editPage;
    PackageModel *m_model;
    int m_oldTab;
    bool m_docksCreated;
    CentralContainer *m_central;
    QString m_currentProject;

    KParts::ReadOnlyPart *m_part;
    KParts::ReadOnlyPart *m_notesPart;
};

#endif // MAINWINDOW_H
