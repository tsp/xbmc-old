#include "pvrclient-mythtv.h"
#include "client.h"



PVRClientMythTV::PVRClientMythTV()
  :m_con(),m_eventHandler(),m_db(),m_protocolVersion(""),m_connectionString(""),m_EPGstart(0),m_EPGend(0),m_channelGroups(),m_categoryMap()
{
  m_categoryMap.insert(catbimap::value_type("Movie",0x10));
  m_categoryMap.insert(catbimap::value_type("Movie", 0x10));
  m_categoryMap.insert(catbimap::value_type("Movie - Detective/Thriller", 0x11));
  m_categoryMap.insert(catbimap::value_type("Movie - Adventure/Western/War", 0x12));
  m_categoryMap.insert(catbimap::value_type("Movie - Science Fiction/Fantasy/Horror", 0x13));
  m_categoryMap.insert(catbimap::value_type("Movie - Comedy", 0x14));
  m_categoryMap.insert(catbimap::value_type("Movie - Soap/melodrama/folkloric", 0x15));
  m_categoryMap.insert(catbimap::value_type("Movie - Romance", 0x16));
  m_categoryMap.insert(catbimap::value_type("Movie - Serious/Classical/Religious/Historical Movie/Drama", 0x17));
  m_categoryMap.insert(catbimap::value_type("Movie - Adult   ", 0x18));
  m_categoryMap.insert(catbimap::value_type("Drama", 0x1F));//MythTV use 0xF0 but xbmc doesn't implement this.
  m_categoryMap.insert(catbimap::value_type("News", 0x20));
  m_categoryMap.insert(catbimap::value_type("News/weather report", 0x21));
  m_categoryMap.insert(catbimap::value_type("News magazine", 0x22));
  m_categoryMap.insert(catbimap::value_type("Documentary", 0x23));
  m_categoryMap.insert(catbimap::value_type("Intelligent Programmes", 0x24));
  m_categoryMap.insert(catbimap::value_type("Entertainment", 0x30));
  m_categoryMap.insert(catbimap::value_type("Game Show", 0x31));
  m_categoryMap.insert(catbimap::value_type("Variety Show", 0x32));
  m_categoryMap.insert(catbimap::value_type("Talk Show", 0x33));
  m_categoryMap.insert(catbimap::value_type("Sports", 0x40));
  m_categoryMap.insert(catbimap::value_type("Special Events (World Cup, World Series, etc)", 0x41));
  m_categoryMap.insert(catbimap::value_type("Sports Magazines", 0x42));
  m_categoryMap.insert(catbimap::value_type("Football (Soccer)", 0x43));
  m_categoryMap.insert(catbimap::value_type("Tennis/Squash", 0x44));
  m_categoryMap.insert(catbimap::value_type("Misc. Team Sports", 0x45));
  m_categoryMap.insert(catbimap::value_type("Athletics", 0x46));
  m_categoryMap.insert(catbimap::value_type("Motor Sport", 0x47));
  m_categoryMap.insert(catbimap::value_type("Water Sport", 0x48));
  m_categoryMap.insert(catbimap::value_type("Winter Sports", 0x49));
  m_categoryMap.insert(catbimap::value_type("Equestrian", 0x4A));
  m_categoryMap.insert(catbimap::value_type("Martial Sports", 0x4B));
  m_categoryMap.insert(catbimap::value_type("Kids", 0x50));
  m_categoryMap.insert(catbimap::value_type("Pre-School Children's Programmes", 0x51));
  m_categoryMap.insert(catbimap::value_type("Entertainment Programmes for 6 to 14", 0x52));
  m_categoryMap.insert(catbimap::value_type("Entertainment Programmes for 10 to 16", 0x53));
  m_categoryMap.insert(catbimap::value_type("Informational/Educational", 0x54));
  m_categoryMap.insert(catbimap::value_type("Cartoons/Puppets", 0x55));
  m_categoryMap.insert(catbimap::value_type("Music/Ballet/Dance", 0x60));
  m_categoryMap.insert(catbimap::value_type("Rock/Pop", 0x61));
  m_categoryMap.insert(catbimap::value_type("Classical Music", 0x62));
  m_categoryMap.insert(catbimap::value_type("Folk Music", 0x63));
  m_categoryMap.insert(catbimap::value_type("Jazz", 0x64));
  m_categoryMap.insert(catbimap::value_type("Musical/Opera", 0x65));
  m_categoryMap.insert(catbimap::value_type("Ballet", 0x66));
  m_categoryMap.insert(catbimap::value_type("Arts/Culture", 0x70));
  m_categoryMap.insert(catbimap::value_type("Performing Arts", 0x71));
  m_categoryMap.insert(catbimap::value_type("Fine Arts", 0x72));
  m_categoryMap.insert(catbimap::value_type("Religion", 0x73));
  m_categoryMap.insert(catbimap::value_type("Popular Culture/Traditional Arts", 0x74));
  m_categoryMap.insert(catbimap::value_type("Literature", 0x75));
  m_categoryMap.insert(catbimap::value_type("Film/Cinema", 0x76));
  m_categoryMap.insert(catbimap::value_type("Experimental Film/Video", 0x77));
  m_categoryMap.insert(catbimap::value_type("Broadcasting/Press", 0x78));
  m_categoryMap.insert(catbimap::value_type("New Media", 0x79));
  m_categoryMap.insert(catbimap::value_type("Arts/Culture Magazines", 0x7A));
  m_categoryMap.insert(catbimap::value_type("Fashion", 0x7B));
  m_categoryMap.insert(catbimap::value_type("Social/Policical/Economics", 0x80));
  m_categoryMap.insert(catbimap::value_type("Magazines/Reports/Documentary", 0x81));
  m_categoryMap.insert(catbimap::value_type("Economics/Social Advisory", 0x82));
  m_categoryMap.insert(catbimap::value_type("Remarkable People", 0x83));
  m_categoryMap.insert(catbimap::value_type("Education/Science/Factual", 0x90));
  m_categoryMap.insert(catbimap::value_type("Nature/animals/Environment", 0x91));
  m_categoryMap.insert(catbimap::value_type("Technology/Natural Sciences", 0x92));
  m_categoryMap.insert(catbimap::value_type("Medicine/Physiology/Psychology", 0x93));
  m_categoryMap.insert(catbimap::value_type("Foreign Countries/Expeditions", 0x94));
  m_categoryMap.insert(catbimap::value_type("Social/Spiritual Sciences", 0x95));
  m_categoryMap.insert(catbimap::value_type("Further Education", 0x96));
  m_categoryMap.insert(catbimap::value_type("Languages", 0x97));
  m_categoryMap.insert(catbimap::value_type("Leisure/Hobbies", 0xA0));
  m_categoryMap.insert(catbimap::value_type("Tourism/Travel", 0xA1));
  m_categoryMap.insert(catbimap::value_type("Handicraft", 0xA2));
  m_categoryMap.insert(catbimap::value_type("Motoring", 0xA3));
  m_categoryMap.insert(catbimap::value_type("Fitness & Health", 0xA4));
  m_categoryMap.insert(catbimap::value_type("Cooking", 0xA5));
  m_categoryMap.insert(catbimap::value_type("Advertizement/Shopping", 0xA6));
  m_categoryMap.insert(catbimap::value_type("Gardening", 0xA7));
  m_categoryMap.insert(catbimap::value_type("Original Language", 0xB0));
  m_categoryMap.insert(catbimap::value_type("Black & White", 0xB1));
  m_categoryMap.insert(catbimap::value_type("\"Unpublished\" Programmes", 0xB2));
  m_categoryMap.insert(catbimap::value_type("Live Broadcast", 0xB3));
}


int PVRClientMythTV::Genre(CStdString g)
{
  int retval=0;
  try{
    retval=m_categoryMap.by< mythcat >().at(g);
  }
  catch(std::out_of_range){}
  return retval;
}
CStdString PVRClientMythTV::Genre(int g)
{
  CStdString retval="";
  try{
    retval=m_categoryMap.by< pvrcat >().at(g);
  }
  catch(std::out_of_range){}
  return retval;
}


void Log(int l,char* msg)
{

  if(msg&&l!=CMYTH_DBG_NONE)
  {
    bool doLog=g_bExtraDebug;
    addon_log_t loglevel=LOG_DEBUG;
    switch( l)
    {
    case CMYTH_DBG_ERROR:
      loglevel=LOG_ERROR;
      doLog=true;
      break;
    case CMYTH_DBG_WARN:
    case CMYTH_DBG_INFO:
      loglevel=LOG_INFO;
      break;
    case CMYTH_DBG_DETAIL:
    case CMYTH_DBG_DEBUG:
    case CMYTH_DBG_PROTO:
    case CMYTH_DBG_ALL: 
      loglevel=LOG_DEBUG;
      break;
    }    
    if(XBMC&&doLog)
      XBMC->Log(loglevel,"LibCMyth: %s",  msg);
  }
}

bool PVRClientMythTV::Connect()
{
  if(g_bExtraDebug)
    CMYTH->DbgAll();
  else
    CMYTH->DbgLevel(CMYTH_DBG_ERROR);
  CMYTH->SetDbgMsgcallback(Log);
  m_con=MythConnection(g_szHostname,g_iMythPort);
  if(!m_con.IsConnected())
  {
    XBMC->Log(LOG_ERROR,"%s: Failed to connect to MythTV backend %s:%i",__FUNCTION__,g_szHostname.c_str(),g_iMythPort);
    return false;
  }
  m_eventHandler=m_con.CreateEventHandler();
  m_protocolVersion.Format("%i",m_con.GetProtocolVersion());
  m_connectionString.Format("%s:%i",g_szHostname,g_iMythPort);
  m_db=MythDatabase(g_szHostname,g_szMythDBname,g_szMythDBuser,g_szMythDBpassword);
  if(m_db.IsNull())
  {
    XBMC->Log(LOG_ERROR,"%s: Failed to connect to MythTV MySQL database %s@%s %s/%s",__FUNCTION__,g_szMythDBname.c_str(),g_szHostname.c_str(),g_szMythDBuser.c_str(),g_szMythDBpassword.c_str());
    return false;
  }
  m_channels=m_db.ChannelList();
  if(m_channels.size()==0)
    XBMC->Log(LOG_INFO,"%s: Empty channellist",__FUNCTION__);
  m_sources=m_db.SourceList();
  if(m_sources.size()==0)
    XBMC->Log(LOG_INFO,"%s: Empty source list",__FUNCTION__);
  m_channelGroups=m_db.GetChannelGroups();
  if(m_channelGroups.size()==0)
    XBMC->Log(LOG_INFO,"%s: No channelgroups",__FUNCTION__);
  return true;
}

const char* PVRClientMythTV::GetBackendName()
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  return m_con.GetServer();
}


const char * PVRClientMythTV::GetBackendVersion()
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  return m_protocolVersion;
}

const char * PVRClientMythTV::GetConnectionString()
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  return m_connectionString;
}

bool PVRClientMythTV::GetDriveSpace(long long *iTotal, long long *iUsed)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  return m_con.GetDriveSpace(*iTotal,*iUsed);
}

PVR_ERROR PVRClientMythTV::GetEPGForChannel(PVR_HANDLE handle, const PVR_CHANNEL &channel, time_t iStart, time_t iEnd)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  if(iStart!=m_EPGstart&&iEnd!=m_EPGend)
  {
    m_EPG=m_db.GetGuide(iStart,iEnd);
    XBMC->Log(LOG_DEBUG,"%s: Fetching EPG - size: %i",__FUNCTION__,m_EPG.size());
    m_EPGstart=iStart;
    m_EPGend=iEnd;
  }
  for(std::vector< MythProgram >::iterator it=m_EPG.begin();it!=m_EPG.end();it++)
  {
    if(it->chanid==channel.iUniqueId)
    {
      EPG_TAG tag;
      tag.endTime=it->endtime;
      tag.iChannelNumber=it->channum;
      tag.startTime=it->starttime;
      tag.strTitle=it->title;
      tag.strPlot= it->description;
      unsigned int seriesid=atoi(it->seriesid);
      if(seriesid!=0)
        tag.iUniqueBroadcastId=atoi(it->seriesid);
      else
        tag.iUniqueBroadcastId=(tag.startTime<<16)+(tag.iChannelNumber&0xffff);
      int genre=Genre(it->category);

      tag.iGenreSubType=genre&0x0F;
      tag.iGenreType=genre&0xF0;
      //unimplemented
      tag.strEpisodeName="";
      tag.strGenreDescription="";
      tag.strIconPath="";
      tag.strPlotOutline="";
      tag.bNotify=false;
      tag.firstAired=0;
      tag.iEpisodeNumber=0;
      tag.iEpisodePartNumber=0;
      tag.iParentalRating=0;
      tag.iSeriesNumber=0;
      tag.iStarRating=0;


      PVR->TransferEpgEntry(handle,&tag);
    }
  }
  return PVR_ERROR_NO_ERROR;
}


int PVRClientMythTV::GetNumChannels()
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  return m_channels.size();
}

PVR_ERROR PVRClientMythTV::GetChannels(PVR_HANDLE handle, bool bRadio)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  for (std::map< int, MythChannel >::iterator it = m_channels.begin(); it != m_channels.end(); it++)
  {
    if (it->second.IsRadio()==bRadio&&!it->second.IsNull())
    {
      PVR_CHANNEL tag;
      tag.bIsHidden=!it->second.Visible();
      tag.bIsRadio=it->second.IsRadio();
      tag.iUniqueId=it->first;
      tag.iChannelNumber=it->second.Number(); //Swap ID and Number?
      CStdString chanName= it->second.Name();
      tag.strChannelName = chanName;
      CStdString icon = it->second.Icon();//TODO: Fix icons
      tag.strIconPath = icon;

      //Unimplemented
      tag.strStreamURL="";
      tag.strInputFormat="";
      tag.iEncryptionSystem=0;


      PVR->TransferChannelEntry(handle,&tag);
    }
  }
  return PVR_ERROR_NO_ERROR;
}

int PVRClientMythTV::GetRecordingsAmount(void)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  m_recordings=m_con.GetRecordedPrograms();
  return m_recordings.size();
}


PVR_ERROR PVRClientMythTV::GetRecordings(PVR_HANDLE handle)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  if(m_recordings.size()==0)
    m_recordings=m_con.GetRecordedPrograms();
  for (boost::unordered_map< CStdString, MythProgramInfo >::iterator it = m_recordings.begin(); it != m_recordings.end(); it++)
  {
    if(!it->second.IsNull())
    {
      PVR_RECORDING tag;
      tag.iDuration=it->second.Duration();
      tag.recordingTime=it->second.RecStart();
      CStdString chanName=it->second.ChannelName();
      CStdString plot=it->second.Description();
      CStdString path=it->second.Path();
      CStdString title=it->second.Title();

      tag.strChannelName=chanName;
      tag.strPlot=plot;
      CStdString id=it->second.Path();
      tag.strRecordingId=id;
      CStdString group=it->second.RecordingGroup();
      tag.strDirectory=group=="Default"?"":group;
      tag.strTitle=title;
      int genre=Genre(it->second.Category());      
      tag.iGenreSubType=genre&0x0F;
      tag.iGenreType=genre&0xF0;

      //Unimplemented
      tag.iLifetime=0;
      tag.iPriority=0;
      tag.strPlotOutline="";
      tag.strStreamURL="";

      PVR->TransferRecordingEntry(handle,&tag);
    }
  }
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR PVRClientMythTV::DeleteRecording(const PVR_RECORDING &recording)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  CStdString id=recording.strRecordingId;
  bool ret = m_con.DeleteRecording(m_recordings.at(id));
  if(ret && m_recordings.erase(recording.strRecordingId))
  {
    PVR->TriggerRecordingUpdate();
    return PVR_ERROR_NO_ERROR;

  }
  else
    return PVR_ERROR_NOT_DELETED;
}

int PVRClientMythTV::GetTimersAmount(void)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  std::vector< MythTimer > m_timers=m_db.GetTimers();
  return m_timers.size();
}

PVR_ERROR PVRClientMythTV::GetTimers(PVR_HANDLE handle)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  std::vector< MythTimer > m_timers=m_db.GetTimers();
  for (std::vector< MythTimer >::iterator it = m_timers.begin(); it != m_timers.end(); it++)
  {
    if(!it->IsNull())
    {
      PVR_TIMER tag;
      tag.endTime=it->EndTime();
      tag.iClientChannelUid=it->ChanID();
      tag.iClientIndex=it->RecordID();
      tag.startTime=it->StartTime();
      CStdString title=it->Title();
      tag.strTitle=title;
      CStdString summary=it->Description(); 
      tag.strSummary=summary;
      tag.state=it->Type()==NotRecording?PVR_TIMER_STATE_CANCELLED:PVR_TIMER_STATE_SCHEDULED;
      int genre=Genre(it->Category());
      tag.iGenreSubType=genre&0x0F;
      tag.iGenreType=genre&0xF0;

      //Unimplemented
      tag.bIsRepeating=false;
      tag.firstDay=0;
      tag.iEpgUid=0;
      tag.iLifetime=0;
      tag.iMarginEnd=0;
      tag.iMarginStart=0;
      tag.iPriority=0;
      tag.iWeekdays=0;
      tag.strDirectory="";

      PVR->TransferTimerEntry(handle,&tag);
    }
  }
  return PVR_ERROR_NO_ERROR;
}


  //typedef enum
  //{
  //  PVR_TIMER_STATE_INVALID   = 0,
  //  PVR_TIMER_STATE_SCHEDULED = 1, /*!< @brief the timer is scheduled for recording */
  //  PVR_TIMER_STATE_RECORDING = 2, /*!< @brief the timer is currently recordings */
  //  PVR_TIMER_STATE_COMPLETED = 3, /*!< @brief the recording completed successfully */
  //  PVR_TIMER_STATE_ABORTED   = 4, /*!< @brief recording started, but was aborted */
  //  PVR_TIMER_STATE_CANCELLED = 5  /*!< @brief the timer was scheduled, but was cancelled */
  //} PVR_TIMER_STATE;

PVR_ERROR PVRClientMythTV::AddTimer(const PVR_TIMER &timer)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);

  CStdString category=Genre(timer.iGenreType);
  TimerType ttype = (timer.state == PVR_TIMER_STATE_ABORTED || PVR_TIMER_STATE_CANCELLED)?NotRecording:SingleRecord;
  int id=m_db.AddTimer(timer.iClientChannelUid,m_channels.at(timer.iClientChannelUid).Name(),timer.strSummary,timer.startTime,timer.endTime,timer.strTitle,category,ttype);
  if(id<0)
    return PVR_ERROR_NOT_POSSIBLE;
  if(!m_con.UpdateSchedules(id))
    return PVR_ERROR_NOT_POSSIBLE;
  PVR->TriggerTimerUpdate();
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR PVRClientMythTV::DeleteTimer(const PVR_TIMER &timer, bool bForceDelete)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  if(!m_db.DeleteTimer(timer.iClientIndex))
    return PVR_ERROR_NOT_POSSIBLE;
  m_con.UpdateSchedules(-1);
  PVR->TriggerTimerUpdate();
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR PVRClientMythTV::UpdateTimer(const PVR_TIMER &timer)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  CStdString category=Genre(timer.iGenreType);
  TimerType ttype = (timer.state == PVR_TIMER_STATE_ABORTED || PVR_TIMER_STATE_CANCELLED)?NotRecording:SingleRecord;
  if(!m_db.UpdateTimer(timer.iClientIndex,timer.iClientChannelUid,m_channels.at(timer.iClientChannelUid).Name(),timer.strSummary,timer.startTime,timer.endTime,timer.strTitle,category,ttype))
    return PVR_ERROR_NOT_POSSIBLE;
  m_con.UpdateSchedules(timer.iClientIndex);
  return PVR_ERROR_NO_ERROR;
}


bool PVRClientMythTV::OpenLiveStream(const PVR_CHANNEL &channel)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  if(m_rec.IsNull())
  {
    MythChannel chan=m_channels.at(channel.iUniqueId);
    for(std::vector<int>::iterator it=m_sources.at(chan.SourceID()).begin();it!=m_sources.at(chan.SourceID()).end();it++)
    {
      m_rec=m_con.GetRecorder(*it);
      if(!m_rec.IsRecording())
      {
        XBMC->Log(LOG_DEBUG,"%s: Opening new recorder %i",__FUNCTION__,m_rec.ID());
        m_eventHandler.SetRecorder(m_rec);
        if(m_rec.SpawnLiveTV(chan))
          return true;
      }
      m_rec=MythRecorder();
      m_eventHandler.SetRecorder(m_rec);
    }
    return false;
  }
  else
    return true;
}


void PVRClientMythTV::CloseLiveStream()
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  m_rec.Stop();
  m_rec=MythRecorder();
  m_eventHandler.SetRecorder(m_rec);
  return;
}

int PVRClientMythTV::ReadLiveStream(unsigned char *pBuffer, unsigned int iBufferSize)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  if(m_rec.IsNull())
    return -1;
  int dataread=m_rec.ReadLiveTV(pBuffer,iBufferSize);
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s: Read %i Bytes",__FUNCTION__,dataread);
  else if(dataread==0)
    XBMC->Log(LOG_INFO,"%s: Read 0 Bytes!",__FUNCTION__);
  return dataread;
}

int PVRClientMythTV::GetCurrentClientChannel()
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  if(m_rec.IsNull())
    return -1;
  MythProgramInfo currentProgram=m_rec.GetCurrentProgram();
  return currentProgram.ChannelID();
}

bool PVRClientMythTV::SwitchChannel(const PVR_CHANNEL &channelinfo)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  MythChannel chan=m_channels.at(channelinfo.iUniqueId);
  bool retval=m_rec.SetChannel(chan);
  if(!retval)
  {
    XBMC->Log(LOG_INFO,"%s - Failed to change to channel: %s(%i) - Reopening Livestream.",__FUNCTION__,channelinfo.strChannelName,channelinfo.iUniqueId);
    CloseLiveStream();
    retval=OpenLiveStream(channelinfo);
    if(!retval)
      XBMC->Log(LOG_ERROR,"%s - Failed to reopening Livestream!",__FUNCTION__);
  }
  return retval;
}


long long PVRClientMythTV::SeekLiveStream(long long iPosition, int iWhence) { 
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  int whence=iWhence==SEEK_SET?WHENCE_SET:iWhence==SEEK_CUR?WHENCE_CUR:WHENCE_END;
  return m_rec.LiveTVSeek(iPosition,whence);
}

long long PVRClientMythTV::LengthLiveStream()
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  return m_rec.LiveTVDuration();
}

PVR_ERROR PVRClientMythTV::SignalStatus(PVR_SIGNAL_STATUS &signalStatus)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  MythSignal signal=m_eventHandler.GetSignal();
  signalStatus.dAudioBitrate=0;
  signalStatus.dDolbyBitrate=0;
  signalStatus.dVideoBitrate=0;
  signalStatus.iBER=signal.BER();
  signalStatus.iSignal=signal.Signal();
  signalStatus.iSNR=signal.SNR();
  signalStatus.iUNC=signal.UNC();
  CStdString ID;
  CStdString adaptorStatus=signal.AdapterStatus();
  ID.Format("Myth Recorder %i",m_rec.ID());
  strcpy(signalStatus.strAdapterName,ID.Buffer());
  strcpy(signalStatus.strAdapterStatus,adaptorStatus.Buffer());
  return PVR_ERROR_NO_ERROR;
}

bool PVRClientMythTV::OpenRecordedStream(const PVR_RECORDING &recinfo)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  CStdString id=recinfo.strRecordingId;
  m_file=m_con.ConnectFile(m_recordings.at(id));

  return !m_file.IsNull();
}

void PVRClientMythTV::CloseRecordedStream()
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  m_file=NULL;
}

int PVRClientMythTV::ReadRecordedStream(unsigned char *pBuffer, unsigned int iBufferSize)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  int dataread=m_file.Read(pBuffer,iBufferSize);
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s: Read %i Bytes",__FUNCTION__,dataread);
  else if(dataread==0)
    XBMC->Log(LOG_INFO,"%s: Read 0 Bytes!",__FUNCTION__);
  return dataread;
}

long long PVRClientMythTV::SeekRecordedStream(long long iPosition, int iWhence)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  int whence=iWhence==SEEK_SET?WHENCE_SET:iWhence==SEEK_CUR?WHENCE_CUR:WHENCE_END;
  return m_file.Seek(iPosition,whence);
}


long long PVRClientMythTV::LengthRecordedStream()
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  return m_file.Duration();
}


int PVRClientMythTV::GetChannelGroupsAmount()
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  return m_channelGroups.size();
}

PVR_ERROR PVRClientMythTV::GetChannelGroups(PVR_HANDLE handle, bool bRadio)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  PVR_CHANNEL_GROUP tag;
  for(boost::unordered_map< CStdString, std::vector< int > >::iterator it=m_channelGroups.begin();it!=m_channelGroups.end();it++)
  {
    tag.strGroupName=it->first;
    tag.bIsRadio=bRadio;
    for(std::vector< int >::iterator it2=it->second.begin();it2!=it->second.end();it2++)
      if(m_channels.find(*it2)!=m_channels.end()&&m_channels.at(*it2).IsRadio()==bRadio)
      {
        PVR->TransferChannelGroup(handle,&tag);
        break;
      }
  }

  return PVR_ERROR_NO_ERROR;

}

PVR_ERROR PVRClientMythTV::GetChannelGroupMembers(PVR_HANDLE handle, const PVR_CHANNEL_GROUP &group)
{
  if(g_bExtraDebug)
    XBMC->Log(LOG_DEBUG,"%s",__FUNCTION__);
  PVR_CHANNEL_GROUP_MEMBER tag;
  int i=0;
  for(std::vector< int >::iterator it=m_channelGroups.at(group.strGroupName).begin();it!=m_channelGroups.at(group.strGroupName).end();it++)
  {
    if(m_channels.find(*it)!=m_channels.end())
    {
      MythChannel chan=m_channels.at(*it); 
      if(group.bIsRadio==chan.IsRadio())
      {
        tag.iChannelNumber=i++;
        tag.iChannelUniqueId=chan.ID();
        tag.strGroupName=group.strGroupName;
        PVR->TransferChannelGroupMember(handle,&tag);
      }
    }
  }
  return PVR_ERROR_NO_ERROR;
}