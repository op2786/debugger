#include <QtGui/QPainter>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include "debuggerwidget.h"
#include "ui.h"

using namespace BinaryNinja;
using namespace std;


DebuggerWidget::DebuggerWidget(const QString& name, ViewFrame* view, BinaryViewRef data):
    SidebarWidget(name), m_view(view), m_data(data)
{
    m_controller = DebuggerController::GetController(m_data);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignTop);

    m_splitter = new QSplitter(Qt::Vertical, this);
    m_splitter->setChildrenCollapsible(true);

    m_breakpointsWidget = new DebugBreakpointsWidget("Native Debugger Breakpoints",
                                                                           m_view, m_data, m_menu);

    m_modulesWidget = new DebugModulesWidget("Native Debugger Modules",
                                                                           m_view, m_data);

    m_threadsWidget = new DebugThreadsWidget("Native Debugger Threads",
                                                                           m_view, m_data);

    m_stackWidget = new DebugStackWidget("Native Debugger Stack",
                                                               m_view, m_data);

    auto bpLayout = new QVBoxLayout();
    bpLayout->setContentsMargins(0, 0, 0, 0);
    bpLayout->addWidget(m_breakpointsWidget);

    auto modulesLayout = new QVBoxLayout();
    modulesLayout->setContentsMargins(0, 0, 0, 0);
    modulesLayout->addWidget(m_modulesWidget);

    auto threadsLayout = new QVBoxLayout();
    threadsLayout->setContentsMargins(0, 0, 0, 0);
    threadsLayout->addWidget(m_threadsWidget);

    auto stackLayout = new QVBoxLayout();
    stackLayout->setContentsMargins(0, 0, 0, 0);
    stackLayout->addWidget(m_stackWidget);

    m_breakpointsGroup = new ExpandableGroup(bpLayout, "Breakpoints");
    m_modulesGroup = new ExpandableGroup(modulesLayout, "Modules");
    m_threadsGroup = new ExpandableGroup(threadsLayout, "Threads");
    m_stackGroup = new ExpandableGroup(stackLayout, "Stack");

    m_splitter->addWidget(m_breakpointsGroup);
    m_splitter->addWidget(m_modulesGroup);
    m_splitter->addWidget(m_threadsGroup);
    m_splitter->addWidget(m_stackGroup);

    layout->addWidget(m_splitter);
    setLayout(layout);

    m_controller->GetUI()->SetDebuggerSidebar(this);

    connect(m_controller, &DebuggerController::contextChanged, this, &DebuggerWidget::updateContext);
}


DebuggerWidget::~DebuggerWidget()
{
//    disconnect(m_ui, &DebuggerUI::contextChanged, 0, 0);
}


void DebuggerWidget::notifyFontChanged()
{
    LogWarn("font changed");
}


void DebuggerWidget::updateContext()
{
    LogWarn("DebuggerWidget::updateContext()");
//    TODO: further refactor this, connect the updateContext signal directly to each of the signals
    m_breakpointsWidget->updateContent();
    m_modulesWidget->updateContent();
    m_threadsWidget->updateContent();
    m_stackWidget->updateContent();
}