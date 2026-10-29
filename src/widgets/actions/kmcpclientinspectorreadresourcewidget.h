/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "actions/kmcpclientinspectoractiontabpagebase.h"
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QHash>
#include <QList>
#include <QSet>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolResource>
class QJsonObject;
class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;
class QTabWidget;
class QTextBrowser;
class JsonPlainTextEditWidget;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolReadResourceResult;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorReadResourceWidget : public KMcpClientInspectorActionTabPageBase
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorReadResourceWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                   QWidget *parent = nullptr);
    ~KMcpClientInspectorReadResourceWidget() override;

    /*!
     * Update resources list from resources/list response \a obj.
     */
    void setResourcesList(const QJsonObject &obj);
    /*!
     * Show resources/read response \a obj.
     */
    void setResult(const QJsonObject &obj);
    /*!
     * Handle resources/subscribe (\a subscribe true) or resources/unsubscribe response \a obj.
     */
    void setSubscriptionResult(const QJsonObject &obj, bool subscribe);

    [[nodiscard]] QList<TextAutoGenerateTextMcpProtocolCore::McpProtocolResource> resources() const;
    [[nodiscard]] QSet<QString> subscribedResources() const;

    /*!
     * Convert \a result to html.
     */
    [[nodiscard]] static QString resultToHtml(const TextAutoGenerateTextMcpProtocolCore::McpProtocolReadResourceResult &result);

private:
    void slotCurrentResourceChanged(int row);
    void slotReadResource();
    void slotSubscribe();
    void slotUnsubscribe();
    void slotResourceUpdated(const QString &uri);
    void clearResources();
    void clearResult();
    void updateButtons();
    void updateResourceItems();
    void showMessage(const QString &message, bool isError);

    QList<TextAutoGenerateTextMcpProtocolCore::McpProtocolResource> mResources;
    QSet<QString> mSubscribedResources;
    // Pending subscribe/unsubscribe requests: request id -> uri
    QHash<qint64, QString> mPendingSubscriptionRequests;
    QPushButton *const mListResourcesButton;
    QListWidget *const mResourcesListWidget;
    QLabel *const mResourceNameLabel;
    QLabel *const mResourceDescriptionLabel;
    QLineEdit *const mUriLineEdit;
    QPushButton *const mReadButton;
    QPushButton *const mSubscribeButton;
    QPushButton *const mUnsubscribeButton;
    QLabel *const mStatusLabel;
    QTabWidget *const mResultTabWidget;
    QTextBrowser *const mContentsBrowser;
    JsonPlainTextEditWidget *const mJsonPlainTextEditWidget;
    qint64 mPendingRequestId = -1;
};
