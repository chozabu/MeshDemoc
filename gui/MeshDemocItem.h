/*
 * Retroshare MeshDemoc Plugin.
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

#ifndef MRK_MESH_DEMOC_MESH_DEMOC_ITEM_H
#define MRK_MESH_DEMOC_MESH_DEMOC_ITEM_H

#include <QMetaType>

#include <interface/rsmeshDemoc.h>
#include "gui/gxs/GxsFeedItem.h"

namespace Ui {
class MeshDemocItem;
}

class RsMeshDemocPost;

class MeshDemocItem : public GxsFeedItem
{
	Q_OBJECT

public:
	MeshDemocItem(FeedHolder *parent, uint32_t feedId, const RsGxsGroupId &groupId, const RsGxsMessageId &messageId, bool isHome, bool autoUpdate);
	MeshDemocItem(FeedHolder *parent, uint32_t feedId, const RsMeshDemocGroup &group, const RsMeshDemocPost &post, bool isHome, bool autoUpdate);
	MeshDemocItem(FeedHolder *parent, uint32_t feedId, const RsMeshDemocPost &post, bool isHome, bool autoUpdate);
	virtual ~MeshDemocItem();

	bool setGroup(const RsMeshDemocGroup& group, bool doFill = true);
	bool setPost(const RsMeshDemocPost& post, bool doFill = true);

	const RsMeshDemocPost &getPost() const;
	RsMeshDemocPost &post();

protected:
	/* FeedItem */
	virtual void doExpand(bool /*open*/) {}

private slots:
	void loadComments();
	void makeUpVote();
	void makeDownVote();
	void readToggled(bool checked);
	void readAndClearItem();

signals:
	void vote(const RsGxsGrpMsgIdPair& msgId, bool up);

protected:
	/* GxsGroupFeedItem */
	virtual QString groupName();
	virtual void loadGroup(const uint32_t &token);
	virtual RetroShareLink::enumType getLinkType() { return RetroShareLink::TYPE_UNKNOWN; }

	/* GxsFeedItem */
	virtual void loadMessage(const uint32_t &token);
	virtual QString messageName();

private:
	void setup();
	void fill();
	void setReadStatus(bool isNew, bool isUnread);

private:
	bool mInFill;

	RsMeshDemocGroup mGroup;
	RsMeshDemocPost mPost;

	/** Qt Designer generated object */
	Ui::MeshDemocItem *ui;
};

Q_DECLARE_METATYPE(RsMeshDemocPost)

#endif
