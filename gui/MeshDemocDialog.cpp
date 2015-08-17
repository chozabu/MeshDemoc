/*
 * Retroshare MeshDemoc Dialog
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

#include "MeshDemocDialog.h"
#include "MeshDemocItem.h"
#include "MeshDemocGroupDialog.h"
#include "MeshDemocListWidget.h"
#include "MeshDemocUserNotify.h"
#include "gui/gxs/GxsGroupShareKey.h"
#include "gui/settings/rsharesettings.h"
#include "gui/common/GroupTreeWidget.h"

#include <interface/rsmeshDemoc.h>

class MeshDemocGroupInfoData : public RsUserdata
{
public:
	MeshDemocGroupInfoData() : RsUserdata() {}

public:
	QMap<RsGxsGroupId, QString> mDescription;
};

/** Constructor */
MeshDemocDialog::MeshDemocDialog(QWidget *parent)
    : GxsGroupFrameDialog(rsMeshDemoc, parent)
{
}

MeshDemocDialog::~MeshDemocDialog()
{
}

UserNotify *MeshDemocDialog::getUserNotify(QObject *parent)
{
	return new MeshDemocUserNotify(rsMeshDemoc, parent);
}

void MeshDemocDialog::loadRequest(const TokenQueue *queue, const TokenRequest &req)
{
}

QString MeshDemocDialog::getHelpString() const
{
	QString hlp_str = tr("<h1><img width=\"32\" src=\":/images/64px_help.png\">&nbsp;&nbsp;MeshDemoc</h1>    \
    <p>The meshDemoc service allows you to share internet links, that spread among Retroshare nodes like forums and \
	 channels</p> \
	 <p>Links can be commented by subscribed users. A promotion system also gives the opportunity to  \
	 enlight important links.</p> \
     <p>There is no restriction on which links are shared. Be careful when clicking on them.</p>\
     <p>MeshDemoc links get deleted after %1 months.</p>\
     ").arg(QString::number(rsMeshDemoc->getStoragePeriod()));

	return hlp_str ;
}

QString MeshDemocDialog::text(TextType type)
{
	switch (type) {
	case TEXT_NAME:
		return tr("MeshDemoc Links");
	case TEXT_NEW:
		return tr("Create Topic");
	case TEXT_TODO:
		return "<b>Open points:</b><ul>"
		       "<li>Subreddits/tag to posts support"
		       "<li>Picture Support"
		       "<li>Navigate channel link"
		       "</ul>";

	case TEXT_YOUR_GROUP:
		return tr("My Topics");
	case TEXT_SUBSCRIBED_GROUP:
		return tr("Subscribed Topics");
	case TEXT_POPULAR_GROUP:
		return tr("Popular Topics");
	case TEXT_OTHER_GROUP:
		return tr("Other Topics");
	}

	return "";
}

QString MeshDemocDialog::icon(IconType type)
{
	switch (type) {
	case ICON_NAME:
		return ":/images/meshDemoc_24.png";
	case ICON_NEW:
		return ":/images/meshDemoc_add_24.png";
	case ICON_YOUR_GROUP:
		return ":/images/folder16.png";
	case ICON_SUBSCRIBED_GROUP:
		return ":/images/folder_red.png";
	case ICON_POPULAR_GROUP:
		return ":/images/folder_green.png";
	case ICON_OTHER_GROUP:
		return ":/images/folder_yellow.png";
	case ICON_DEFAULT:
		return "";
	}

	return "";
}

GxsGroupDialog *MeshDemocDialog::createNewGroupDialog(TokenQueue *tokenQueue)
{
	return new MeshDemocGroupDialog(tokenQueue, this);
}

GxsGroupDialog *MeshDemocDialog::createGroupDialog(TokenQueue *tokenQueue, RsTokenService *tokenService, GxsGroupDialog::Mode mode, RsGxsGroupId groupId)
{
	return new MeshDemocGroupDialog(tokenQueue, tokenService, mode, groupId, this);
}

int MeshDemocDialog::shareKeyType()
{
	return POSTED_KEY_SHARE;
	//return MESH_DEMOC_KEY_SHARE;
}

GxsMessageFrameWidget *MeshDemocDialog::createMessageFrameWidget(const RsGxsGroupId &groupId)
{
	return new MeshDemocListWidget(groupId);
}

RsGxsCommentService *MeshDemocDialog::getCommentService()
{
	return rsMeshDemoc;
}

QWidget *MeshDemocDialog::createCommentHeaderWidget(const RsGxsGroupId &grpId, const RsGxsMessageId &msgId)
{
	return new MeshDemocItem(NULL, 0, grpId, msgId, true, false);
}

void MeshDemocDialog::loadGroupSummaryToken(const uint32_t &token, std::list<RsGroupMetaData> &groupInfo, RsUserdata *&userdata)
{
	std::vector<RsMeshDemocGroup> groups;
	rsMeshDemoc->getGroupData(token, groups);

	/* Save groups to fill description */
	MeshDemocGroupInfoData *meshDemocData = new MeshDemocGroupInfoData;
	userdata = meshDemocData;

	std::vector<RsMeshDemocGroup>::iterator groupIt;
	for (groupIt = groups.begin(); groupIt != groups.end(); ++groupIt) {
		RsMeshDemocGroup &group = *groupIt;
		groupInfo.push_back(group.mMeta);

		if (!group.mDescription.empty()) {
			meshDemocData->mDescription[group.mMeta.mGroupId] = QString::fromUtf8(group.mDescription.c_str());
		}
	}
}

void MeshDemocDialog::groupInfoToGroupItemInfo(const RsGroupMetaData &groupInfo, GroupItemInfo &groupItemInfo, const RsUserdata *userdata)
{
	GxsGroupFrameDialog::groupInfoToGroupItemInfo(groupInfo, groupItemInfo, userdata);

	const MeshDemocGroupInfoData *meshDemocData = dynamic_cast<const MeshDemocGroupInfoData*>(userdata);
	if (!meshDemocData) {
		std::cerr << "MeshDemocDialog::groupInfoToGroupItemInfo() Failed to cast data to MeshDemocGroupInfoData";
		std::cerr << std::endl;
		return;
	}

	QMap<RsGxsGroupId, QString>::const_iterator descriptionIt = meshDemocData->mDescription.find(groupInfo.mGroupId);
	if (descriptionIt != meshDemocData->mDescription.end()) {
		groupItemInfo.description = descriptionIt.value();
	}
}
