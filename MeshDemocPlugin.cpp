#include <retroshare/rsplugin.h>
#include <util/rsversioninfo.h>
#include <QTranslator>
#include <QIcon>

#include "MeshDemocPlugin.h"
#include "gui/MeshDemocDialog.h"
//#include "LinksDialog.h"
#include "topjcdialog.h"
#include <util/rsdir.h>

#include "services/p3meshDemoc.h"
#include "gxs/rsdataservice.h"
#include "gxs/rsgxsnetservice.h"

//static void *inited = new MeshDemocPlugin();

//inited->rs_cache_service();

extern "C" {
	void *RETROSHARE_PLUGIN_provide()
	{
		static MeshDemocPlugin *p = new MeshDemocPlugin() ;

		return (void*)p ;
	}

	// This symbol contains the svn revision number grabbed from the executable. 
	// It will be tested by RS to load the plugin automatically, since it is safe to load plugins
	// with same revision numbers, assuming that the revision numbers are up-to-date.
	//
	uint32_t RETROSHARE_PLUGIN_revision = RS_REVISION_NUMBER ;

	// This symbol contains the svn revision number grabbed from the executable. 
	// It will be tested by RS to load the plugin automatically, since it is safe to load plugins
	// with same revision numbers, assuming that the revision numbers are up-to-date.
	//
	uint32_t RETROSHARE_PLUGIN_api = RS_PLUGIN_API_VERSION ;
}

#define IMAGE_LINKS ":/images/dice.png"

void MeshDemocPlugin::getPluginVersion(int& major, int& minor, int &build, int& svn_rev) const
{
	major = RS_MAJOR_VERSION;
	minor = RS_MINOR_VERSION;
	build = RS_BUILD_NUMBER;
	svn_rev = RS_REVISION_NUMBER;
}

MeshDemocPlugin::MeshDemocPlugin()
{
    //mRanking = NULL ;
	mainpage = NULL ;
        mIcon = NULL ;
        mPlugInHandler = NULL;
        mPeers = NULL;
        mFiles = NULL;
}

void MeshDemocPlugin::setInterfaces(RsPlugInInterfaces &interfaces){

    mPeers = interfaces.mPeers;
    mFiles = interfaces.mFiles;


    std::string dataDir = interfaces.mGxsDir + "meshDemoc";
    RsDirUtil::checkCreateDirectory(dataDir);
    RsGeneralDataService* wall_ds;
    wall_ds = new RsDataService(dataDir, "meshDemoc_db",
                                RS_SERVICE_TYPE_MESH_DEMOC, NULL, "todo: encrypt db with secure password");
    //wall = new RsWall::p3WallService(wall_ds, NULL, interfaces.mGxsIdService, interfaces.mIdentity);
    p3MeshDemoc *mMeshDemoc = new p3MeshDemoc(wall_ds, NULL, interfaces.mGxsIdService);
    //mMeshDemoc = new p3MeshDemoc(wall_ds, NULL, interfaces.mGxsIdService);
    RsGxsNetService *wall_ns = new RsGxsNetService(
                RS_SERVICE_TYPE_MESH_DEMOC, wall_ds, interfaces.mRsNxsNetMgr,
                mMeshDemoc, mMeshDemoc->getServiceInfo(),
                interfaces.mGxsIdService, interfaces.mGxsCirlces, interfaces.mPgpAuxUtils,
                true    // group auto sync
            );
    rsMeshDemoc = mMeshDemoc;
    mMeshDemoc->start();
    wall_ns->start();
}

MainPage *MeshDemocPlugin::qt_page() const
{
	if(mainpage == NULL)
				mainpage = new MeshDemocDialog();

	return mainpage ;
}

RsCacheService *MeshDemocPlugin::rs_cache_service() const
{
    /*if(mRanking == NULL)
	{
                mRanking = new p3Ranking(mPlugInHandler) ; // , 3600 * 24 * 30 * 6); // 6 Months
		rsRanks = mRanking ;
	}

    return mRanking ;*/
    return NULL;
}

void MeshDemocPlugin::setPlugInHandler(RsPluginHandler *pgHandler){
    mPlugInHandler = pgHandler;

}

QIcon *MeshDemocPlugin::qt_icon() const
{
	if(mIcon == NULL)
	{
		Q_INIT_RESOURCE(MeshDemoc_images) ;

        mIcon = new QIcon(IMAGE_LINKS) ;
	}

	return mIcon ;
}

std::string MeshDemocPlugin::getShortPluginDescription() const
{
	return "infosoon";//QApplication::translate("MeshDemocPlugin", "This plugin provides a set of cached links, and a voting system to promote them.").toUtf8().constData();
}

std::string MeshDemocPlugin::getPluginName() const
{
	return "MeshDemoc";//QApplication::translate("MeshDemocPlugin", "MeshDemoc").toUtf8().constData();
}

QTranslator* MeshDemocPlugin::qt_translator(QApplication */*app*/, const QString& languageCode, const QString& externalDir) const
{
	if (languageCode == "en") {
		return NULL;
	}

	QTranslator* translator = new QTranslator();

	if (translator->load(externalDir + "/MeshDemoc_" + languageCode + ".qm")) {
		return translator;
	} else if (translator->load(":/lang/MeshDemoc_" + languageCode + ".qm")) {
		return translator;
	}

	delete(translator);
	return NULL;
}
