/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorcompletewidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <KLocalizedString>
#include <QComboBox>
#include <QFormLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QRegularExpression>
#include <QSplitter>
#include <QTabWidget>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolCompleteRequestParams>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolCompleteResult>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolError>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolListPromptsResult>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolListResourceTemplatesResult>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolPromptReference>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolResourceTemplateReference>
using namespace Qt::Literals::StringLiterals;
using namespace TextAutoGenerateTextMcpProtocolCore;

KMcpClientInspectorCompleteWidget::KMcpClientInspectorCompleteWidget(McpProtocolClientProtocolManager *protocolManager, QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
    , mReferenceTypeComboBox(new QComboBox(this))
    , mReferenceComboBox(new QComboBox(this))
    , mArgumentNameComboBox(new QComboBox(this))
    , mArgumentValueLineEdit(new QLineEdit(this))
    , mContextLineEdit(new QLineEdit(this))
    , mCompleteButton(new QPushButton(i18nc("@action:button", "Complete"), this))
    , mStatusLabel(new QLabel(this))
    , mResultTabWidget(new QTabWidget(this))
    , mValuesListWidget(new QListWidget(this))
    , mJsonPlainTextEditWidget(new JsonPlainTextEditWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto splitter = new QSplitter(Qt::Vertical, this);
    splitter->setObjectName(u"splitter"_s);
    splitter->setChildrenCollapsible(false);
    mainLayout->addWidget(splitter);

    // Request
    auto requestWidget = new QWidget(splitter);
    auto requestLayout = new QVBoxLayout(requestWidget);
    requestLayout->setObjectName(u"requestLayout"_s);
    requestLayout->setContentsMargins({});

    auto formLayout = new QFormLayout;
    formLayout->setObjectName(u"formLayout"_s);
    requestLayout->addLayout(formLayout);

    mReferenceTypeComboBox->setObjectName(u"mReferenceTypeComboBox"_s);
    mReferenceTypeComboBox->addItem(i18nc("@item:inlistbox completion reference", "Prompt"), QVariant::fromValue(ReferenceType::Prompt));
    mReferenceTypeComboBox->addItem(i18nc("@item:inlistbox completion reference", "Resource Template"), QVariant::fromValue(ReferenceType::ResourceTemplate));
    formLayout->addRow(i18nc("@label:listbox", "Reference type:"), mReferenceTypeComboBox);

    mReferenceComboBox->setObjectName(u"mReferenceComboBox"_s);
    mReferenceComboBox->setEditable(true);
    mReferenceComboBox->setInsertPolicy(QComboBox::NoInsert);
    formLayout->addRow(i18nc("@label:listbox", "Reference:"), mReferenceComboBox);

    mArgumentNameComboBox->setObjectName(u"mArgumentNameComboBox"_s);
    mArgumentNameComboBox->setEditable(true);
    mArgumentNameComboBox->setInsertPolicy(QComboBox::NoInsert);
    formLayout->addRow(i18nc("@label:listbox", "Argument:"), mArgumentNameComboBox);

    mArgumentValueLineEdit->setObjectName(u"mArgumentValueLineEdit"_s);
    mArgumentValueLineEdit->setClearButtonEnabled(true);
    mArgumentValueLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Value to complete"));
    formLayout->addRow(i18nc("@label:textbox", "Value:"), mArgumentValueLineEdit);

    mContextLineEdit->setObjectName(u"mContextLineEdit"_s);
    mContextLineEdit->setClearButtonEnabled(true);
    mContextLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Already resolved arguments, e.g. {\"language\": \"cpp\"}"));
    formLayout->addRow(i18nc("@label:textbox", "Context:"), mContextLineEdit);

    auto completeLayout = new QHBoxLayout;
    completeLayout->setObjectName(u"completeLayout"_s);
    mCompleteButton->setObjectName(u"mCompleteButton"_s);
    completeLayout->addWidget(mCompleteButton);
    mStatusLabel->setObjectName(u"mStatusLabel"_s);
    mStatusLabel->setWordWrap(true);
    mStatusLabel->setTextFormat(Qt::PlainText);
    completeLayout->addWidget(mStatusLabel, 1);
    requestLayout->addLayout(completeLayout);
    requestLayout->addStretch(1);
    splitter->addWidget(requestWidget);

    // Result
    mResultTabWidget->setObjectName(u"mResultTabWidget"_s);
    mValuesListWidget->setObjectName(u"mValuesListWidget"_s);
    mValuesListWidget->setToolTip(i18nc("@info:tooltip", "Double-click on a value to use it"));
    mJsonPlainTextEditWidget->setObjectName(u"mJsonPlainTextEditWidget"_s);
    mResultTabWidget->addTab(mValuesListWidget, i18nc("@title:tab", "Values"));
    mResultTabWidget->addTab(mJsonPlainTextEditWidget, i18nc("@title:tab", "JSON"));
    splitter->addWidget(mResultTabWidget);
    splitter->setStretchFactor(1, 1);

    connect(mReferenceTypeComboBox, &QComboBox::currentIndexChanged, this, &KMcpClientInspectorCompleteWidget::updateReferences);
    connect(mReferenceComboBox, &QComboBox::currentTextChanged, this, &KMcpClientInspectorCompleteWidget::updateArgumentNames);
    connect(mReferenceComboBox, &QComboBox::currentTextChanged, this, &KMcpClientInspectorCompleteWidget::updateCompleteButton);
    connect(mArgumentNameComboBox, &QComboBox::currentTextChanged, this, &KMcpClientInspectorCompleteWidget::updateCompleteButton);
    connect(mArgumentValueLineEdit, &QLineEdit::returnPressed, this, &KMcpClientInspectorCompleteWidget::slotComplete);
    connect(mCompleteButton, &QPushButton::clicked, this, &KMcpClientInspectorCompleteWidget::slotComplete);
    connect(mValuesListWidget, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item) {
        mArgumentValueLineEdit->setText(item->text());
    });

    connect(mProtocolManager, &McpProtocolClientProtocolManager::initialized, this, &KMcpClientInspectorCompleteWidget::updateCompleteButton);
    connect(mProtocolManager, &McpProtocolClientProtocolManager::finished, this, [this]() {
        // Reset pending request first, otherwise result is not cleared
        setRequestInProgress(false);
        mPrompts.clear();
        mResourceTemplates.clear();
        updateReferences();
        clearResult();
    });

    updateReferences();
}

KMcpClientInspectorCompleteWidget::~KMcpClientInspectorCompleteWidget() = default;

QList<McpProtocolPrompt> KMcpClientInspectorCompleteWidget::prompts() const
{
    return mPrompts;
}

QList<McpProtocolResourceTemplate> KMcpClientInspectorCompleteWidget::resourceTemplates() const
{
    return mResourceTemplates;
}

KMcpClientInspectorCompleteWidget::ReferenceType KMcpClientInspectorCompleteWidget::referenceType() const
{
    return mReferenceTypeComboBox->currentData().value<ReferenceType>();
}

void KMcpClientInspectorCompleteWidget::setPromptsList(const QJsonObject &obj)
{
    if (!obj.value("result"_L1).isObject()) {
        // Error response: keep current list
        return;
    }
    const McpProtocolListPromptsResult result = McpProtocolListPromptsResult::fromJson(obj.value("result"_L1).toObject());
    mPrompts.clear();
    const QList<McpProtocolPrompt> prompts = result.prompts();
    for (const McpProtocolPrompt &prompt : prompts) {
        if (!prompt.name().isEmpty()) {
            mPrompts.append(prompt);
        }
    }
    std::stable_sort(mPrompts.begin(), mPrompts.end(), [](const McpProtocolPrompt &left, const McpProtocolPrompt &right) {
        return left.name().localeAwareCompare(right.name()) < 0;
    });
    if (referenceType() == ReferenceType::Prompt) {
        updateReferences();
    }
}

void KMcpClientInspectorCompleteWidget::setResourceTemplatesList(const QJsonObject &obj)
{
    if (!obj.value("result"_L1).isObject()) {
        // Error response: keep current list
        return;
    }
    const McpProtocolListResourceTemplatesResult result = McpProtocolListResourceTemplatesResult::fromJson(obj.value("result"_L1).toObject());
    mResourceTemplates.clear();
    const QList<McpProtocolResourceTemplate> resourceTemplates = result.resourceTemplates();
    for (const McpProtocolResourceTemplate &resourceTemplate : resourceTemplates) {
        if (!resourceTemplate.uriTemplate().isEmpty()) {
            mResourceTemplates.append(resourceTemplate);
        }
    }
    std::stable_sort(mResourceTemplates.begin(),
                     mResourceTemplates.end(),
                     [](const McpProtocolResourceTemplate &left, const McpProtocolResourceTemplate &right) {
                         return left.uriTemplate().localeAwareCompare(right.uriTemplate()) < 0;
                     });
    if (referenceType() == ReferenceType::ResourceTemplate) {
        updateReferences();
    }
}

void KMcpClientInspectorCompleteWidget::updateReferences()
{
    // Keep reference typed or selected by user when list is updated
    const QString currentReference = mReferenceComboBox->currentText();
    {
        const QSignalBlocker blocker(mReferenceComboBox);
        mReferenceComboBox->clear();
        switch (referenceType()) {
        case ReferenceType::Prompt:
            for (const McpProtocolPrompt &prompt : std::as_const(mPrompts)) {
                mReferenceComboBox->addItem(prompt.name());
                mReferenceComboBox->setItemData(mReferenceComboBox->count() - 1, prompt.description().value_or(QString()), Qt::ToolTipRole);
            }
            mReferenceComboBox->lineEdit()->setPlaceholderText(i18nc("@info:placeholder", "Prompt name"));
            break;
        case ReferenceType::ResourceTemplate:
            for (const McpProtocolResourceTemplate &resourceTemplate : std::as_const(mResourceTemplates)) {
                mReferenceComboBox->addItem(resourceTemplate.uriTemplate());
                mReferenceComboBox->setItemData(mReferenceComboBox->count() - 1, resourceTemplate.name(), Qt::ToolTipRole);
            }
            mReferenceComboBox->lineEdit()->setPlaceholderText(i18nc("@info:placeholder", "Resource template uri, e.g. file:///{path}"));
            break;
        }
        const int index = mReferenceComboBox->findText(currentReference);
        if (index >= 0) {
            mReferenceComboBox->setCurrentIndex(index);
        } else if (mReferenceComboBox->count() > 0) {
            mReferenceComboBox->setCurrentIndex(0);
        } else {
            mReferenceComboBox->setEditText({});
        }
    }
    updateArgumentNames();
    updateCompleteButton();
}

void KMcpClientInspectorCompleteWidget::updateArgumentNames()
{
    const QString currentArgument = mArgumentNameComboBox->currentText();
    const QString reference = mReferenceComboBox->currentText();
    QStringList names;
    switch (referenceType()) {
    case ReferenceType::Prompt:
        for (const McpProtocolPrompt &prompt : std::as_const(mPrompts)) {
            if (prompt.name() == reference) {
                const QList<McpProtocolPromptArgument> arguments = prompt.arguments().value_or(QList<McpProtocolPromptArgument>{});
                for (const McpProtocolPromptArgument &argument : arguments) {
                    names.append(argument.name());
                }
                break;
            }
        }
        break;
    case ReferenceType::ResourceTemplate:
        names = uriTemplateVariables(reference);
        break;
    }

    const QSignalBlocker blocker(mArgumentNameComboBox);
    mArgumentNameComboBox->clear();
    mArgumentNameComboBox->addItems(names);
    if (const int index = mArgumentNameComboBox->findText(currentArgument); index >= 0) {
        mArgumentNameComboBox->setCurrentIndex(index);
    } else if (!names.isEmpty()) {
        mArgumentNameComboBox->setCurrentIndex(0);
    } else {
        mArgumentNameComboBox->setEditText(currentArgument);
    }
    updateCompleteButton();
}

QStringList KMcpClientInspectorCompleteWidget::uriTemplateVariables(const QString &uriTemplate)
{
    static const QRegularExpression expressionRegExp(u"\\{([^}]*)\\}"_s);
    QStringList variables;
    QRegularExpressionMatchIterator it = expressionRegExp.globalMatch(uriTemplate);
    while (it.hasNext()) {
        QString expression = it.next().captured(1);
        // Remove operator (RFC 6570)
        if (!expression.isEmpty() && u"+#./;?&"_s.contains(expression.at(0))) {
            expression.remove(0, 1);
        }
        const QStringList specs = expression.split(u',', Qt::SkipEmptyParts);
        for (QString spec : specs) {
            // Remove modifiers: prefix (":3") and explode ("*")
            if (const int colon = spec.indexOf(u':'); colon >= 0) {
                spec.truncate(colon);
            }
            if (spec.endsWith(u'*')) {
                spec.chop(1);
            }
            spec = spec.trimmed();
            if (!spec.isEmpty() && !variables.contains(spec)) {
                variables.append(spec);
            }
        }
    }
    return variables;
}

std::optional<McpProtocolCompleteRequestParams> KMcpClientInspectorCompleteWidget::requestParams(QString &errorMessage) const
{
    const QString reference = mReferenceComboBox->currentText().trimmed();
    if (reference.isEmpty()) {
        errorMessage = i18n("A reference is required.");
        return std::nullopt;
    }
    const QString argumentName = mArgumentNameComboBox->currentText().trimmed();
    if (argumentName.isEmpty()) {
        errorMessage = i18n("An argument name is required.");
        return std::nullopt;
    }

    McpProtocolCompleteRequestParams params;
    switch (referenceType()) {
    case ReferenceType::Prompt: {
        McpProtocolPromptReference promptReference;
        promptReference.setName(reference);
        params.setRef(promptReference);
        break;
    }
    case ReferenceType::ResourceTemplate: {
        McpProtocolResourceTemplateReference resourceTemplateReference;
        resourceTemplateReference.setUri(reference);
        params.setRef(resourceTemplateReference);
        break;
    }
    }

    McpProtocolCompleteRequestParams::Argument argument;
    argument.setName(argumentName);
    argument.setValue(mArgumentValueLineEdit->text());
    params.setArgument(argument);

    if (const QString contextText = mContextLineEdit->text().trimmed(); !contextText.isEmpty()) {
        QJsonParseError parseError;
        const QJsonDocument doc = QJsonDocument::fromJson(contextText.toUtf8(), &parseError);
        if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
            errorMessage = i18n("Context must be a JSON object, e.g. {\"language\": \"cpp\"}.");
            return std::nullopt;
        }
        QMap<QString, QString> contextArguments;
        const QJsonObject contextObject = doc.object();
        for (auto it = contextObject.constBegin(); it != contextObject.constEnd(); ++it) {
            if (!it.value().isString()) {
                errorMessage = i18n("Context value of \"%1\" must be a string.", it.key());
                return std::nullopt;
            }
            contextArguments.insert(it.key(), it.value().toString());
        }
        McpProtocolCompleteRequestParams::Context context;
        context.setArguments(contextArguments);
        params.setContext(context);
    }
    return params;
}

void KMcpClientInspectorCompleteWidget::clearResult()
{
    mStatusLabel->clear();
    mValuesListWidget->clear();
    mJsonPlainTextEditWidget->setPlainText({});
}

void KMcpClientInspectorCompleteWidget::updateCompleteButton()
{
    mCompleteButton->setEnabled(mPendingRequestId < 0 && mProtocolManager->isInitialized() && !mReferenceComboBox->currentText().trimmed().isEmpty()
                                && !mArgumentNameComboBox->currentText().trimmed().isEmpty());
}

void KMcpClientInspectorCompleteWidget::setRequestInProgress(bool inProgress)
{
    if (!inProgress) {
        mPendingRequestId = -1;
    }
    updateCompleteButton();
}

void KMcpClientInspectorCompleteWidget::showMessage(const QString &message, bool isError)
{
    mStatusLabel->setText(message);
    QPalette palette = mStatusLabel->palette();
    palette.setColor(QPalette::WindowText, isError ? QColor(Qt::red) : this->palette().color(QPalette::WindowText));
    mStatusLabel->setPalette(palette);
}

void KMcpClientInspectorCompleteWidget::slotComplete()
{
    if (mPendingRequestId >= 0) {
        return;
    }
    clearResult();
    QString errorMessage;
    const std::optional<McpProtocolCompleteRequestParams> params = requestParams(errorMessage);
    if (!params.has_value()) {
        showMessage(errorMessage, true);
        return;
    }
    const qint64 requestId = mProtocolManager->complete(*params);
    if (requestId < 0) {
        showMessage(i18n("Completion cannot be requested. Is the server connected and does it support completions?"), true);
        return;
    }
    mPendingRequestId = requestId;
    showMessage(i18n("Requesting completion…"), false);
    updateCompleteButton();
}

void KMcpClientInspectorCompleteWidget::setResult(const QJsonObject &obj)
{
    setRequestInProgress(false);
    mJsonPlainTextEditWidget->setPlainText(QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Indented)));
    mValuesListWidget->clear();
    if (const QJsonValue errorValue = obj.value("error"_L1); errorValue.isObject()) {
        const McpProtocolError error = McpProtocolError::fromJson(errorValue.toObject());
        showMessage(i18n("Error %1: %2", QString::number(error.code()), error.message()), true);
        mResultTabWidget->setCurrentWidget(mJsonPlainTextEditWidget);
        return;
    }
    const McpProtocolCompleteResult result = McpProtocolCompleteResult::fromJson(obj.value("result"_L1).toObject());
    const McpProtocolCompleteResult::Completion completion = result.completion();
    const QStringList values = completion.values();
    mValuesListWidget->addItems(values);

    QString message = i18np("%1 value.", "%1 values.", values.count());
    if (const std::optional<int> total = completion.total(); total.has_value()) {
        message += u' ' + i18n("Total: %1.", *total);
    }
    if (completion.hasMore().value_or(false)) {
        message += u' ' + i18n("More values are available.");
    }
    showMessage(message, false);
    mResultTabWidget->setCurrentWidget(mValuesListWidget);
}

#include "moc_kmcpclientinspectorcompletewidget.cpp"
