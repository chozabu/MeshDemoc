/*
 * Retroshare MeshDemoc List
 *
 * Copyright 2012-2012 by Robert Fernie.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "retroshare@lunamutt.com".
 *
 */

#ifndef MESH_DEMOC_LIST_WIDGET_H
#define MESH_DEMOC_LIST_WIDGET_H

#include <QMap>

#include "gui/gxs/GxsMessageFramePostWidget.h"
#include "gui/feeds/FeedHolder.h"

class RsMeshDemocGroup;
class RsMeshDemocPost;
class MeshDemocItem;

namespace Ui {
class MeshDemocListWidget;
}

class MeshDemocListWidget : public GxsMessageFramePostWidget, public FeedHolder
{
	Q_OBJECT

public:
	MeshDemocListWidget(const RsGxsGroupId &meshDemocId, QWidget *parent = 0);
	~MeshDemocListWidget();

	/* GxsMessageFrameWidget */
	virtual QIcon groupIcon();

	/* FeedHolder */
	virtual QScrollArea *getScrollArea();
	virtual void deleteFeedItem(QWidget *item, uint32_t type);
	virtual void openChat(const RsPeerId& peerId);
	virtual void openComments(uint32_t type, const RsGxsGroupId &groupId, const RsGxsMessageId &msgId, const QString &title);

	/* GXS functions */
	virtual void loadRequest(const TokenQueue *queue, const TokenRequest &req);

protected:
	/* GxsMessageFramePostWidget */
	virtual bool insertGroupData(const uint32_t &token, RsGroupMetaData &metaData);
	virtual void insertAllPosts(const uint32_t &token, GxsMessageFramePostThread *thread);
	virtual void insertPosts(const uint32_t &token);
	virtual void insertRelatedPosts(const uint32_t &token);
	virtual void clearPosts();
	virtual bool navigatePostItem(const RsGxsMessageId& msgId);

	/* GxsMessageFrameWidget */
	virtual void setAllMessagesReadDo(bool read, uint32_t &token);

private slots:
	void newPost();

	void submitVote(const RsGxsGrpMsgIdPair& msgId, bool up);

	void getRankings();

	void subscribeGroup(bool subscribe);

	void showNext();
	void showPrev();

private:
	void processSettings(bool load);
	void updateShowText();

	/*!
	 * Only removes it from layout
	 */
	void shallowClearPosts();

	void loadPost(const RsMeshDemocPost &post);

	void insertMeshDemocDetails(const RsMeshDemocGroup &group);

	// subscribe/unsubscribe ack.
//	void acknowledgeSubscribeChange(const uint32_t &token);

	// votes
	void acknowledgeVoteMsg(const uint32_t& token);
	void loadVoteData(const uint32_t &token);

	// ranking
	//void loadRankings(const uint32_t& token);
	//void applyRanking(const MeshDemocRanking& ranks);
	void applyRanking();

private:
	int	mSortMethod;
	int	mLastSortMethod;
	int	mPostIndex;
	int	mPostShow;

	uint32_t mTokenTypeVote;

	QMap<RsGxsMessageId, MeshDemocItem*> mPosts;
	QList<MeshDemocItem*> mPostItems;

	/* UI - from Designer */
	Ui::MeshDemocListWidget *ui;
};

#endif
