#include "cppmyth.h"
#include <sstream>
#include "client.h"
#include "thread.h"
/*
 *             MythPointer template
 */

template <class T> class MythPointer
{
public:
  ~MythPointer()
  {
    CMYTH->RefRelease(m_mythpointer);
    m_mythpointer=0;
  }
  MythPointer()
  {
    m_mythpointer=0;
  }
  operator T()
  {
    return m_mythpointer;
  }
  MythPointer & operator=(const T mythpointer)
  {
        m_mythpointer=mythpointer;
        return *this;
  }
protected:
  T m_mythpointer;
};

/*
 *             MythPointerThreadSafe template
 */

template <class T> class MythPointerThreadSafe : public cMutex, MythPointer<T>
{
public:
  operator T()
  {
    return m_mythpointer;
  }
  MythPointerThreadSafe & operator=(const T mythpointer)
  {
        m_mythpointer=mythpointer;
        return *this;
  }
};

/*
 *            MythSignal
 */
  
    MythSignal::MythSignal() : m_AdapterName(),m_AdapterStatus(),m_SNR(0),m_Signal(0),m_BER(0),m_UNC(0),m_VideoBitrate(0.0),m_AudioBitrate(0.0),m_DolbyBitrate(0.0)    {}

    CStdString  MythSignal::AdapterName(){return m_AdapterName;}       /*!< @brief (optional) name of the adapter that's being used */
    CStdString  MythSignal::AdapterStatus(){return m_AdapterStatus;}     /*!< @brief (optional) status of the adapter that's being used */
    int    MythSignal::SNR(){return m_SNR;}                       /*!< @brief (optional) signal/noise ratio */
    int    MythSignal::Signal(){return m_Signal;}                    /*!< @brief (optional) signal strength */
    long   MythSignal::BER(){return m_BER;}                       /*!< @brief (optional) bit error rate */
    long   MythSignal::UNC(){return m_UNC;}                       /*!< @brief (optional) uncorrected blocks */
    double MythSignal::VideoBitrate(){return m_VideoBitrate;}              /*!< @brief (optional) video bitrate */
    double MythSignal::AudioBitrate(){return m_AudioBitrate;}              /*!< @brief (optional) audio bitrate */
    double MythSignal::DolbyBitrate(){return m_DolbyBitrate;}              /*!< @brief (optional) dolby bitrate */

/*
*								MythEventHandler
*/

class MythEventHandler::ImpMythEventHandler : public cThread
{
public:
  ImpMythEventHandler(CStdString server,unsigned short port);
  MythRecorder m_rec;
  MythSignal m_signal;
  virtual void Action(void);	
  cmyth_conn_t m_conn_t;
  virtual ~ImpMythEventHandler();
  };

MythEventHandler::ImpMythEventHandler::ImpMythEventHandler(CStdString server,unsigned short port)
:m_rec(0),m_conn_t(0),cThread("MythEventHandler"),m_signal()
  {
    char *cserver=_strdup(server.c_str());
    cmyth_conn_t connection=CMYTH->ConnConnectEvent(cserver,port,64*1024, 16*1024);
    free(cserver);
    m_conn_t=connection; 
  }


  MythEventHandler::ImpMythEventHandler::~ImpMythEventHandler()
  {
    Cancel(30);
    CMYTH->RefRelease(m_conn_t);
    m_conn_t=0;
  }

MythEventHandler::MythEventHandler(CStdString server,unsigned short port)
  :m_imp(new ImpMythEventHandler(server,port))
{
  m_imp->Start();
}

MythEventHandler::MythEventHandler()
  :m_imp()
{
}

void MythEventHandler::SetRecorder(MythRecorder &rec)
{
  
  m_imp->m_rec=rec;
}

MythSignal MythEventHandler::GetSignal()
{
  return m_imp->m_signal;
}


void MythEventHandler::ImpMythEventHandler::Action(void)
{
  const char* events[]={	"CMYTH_EVENT_UNKNOWN",\
    "CMYTH_EVENT_CLOSE",\
    "CMYTH_EVENT_RECORDING_LIST_CHANGE",\
    "CMYTH_EVENT_RECORDING_LIST_CHANGE_ADD",\
    "CMYTH_EVENT_RECORDING_LIST_CHANGE_UPDATE",\
    "CMYTH_EVENT_RECORDING_LIST_CHANGE_DELETE",\
    "CMYTH_EVENT_SCHEDULE_CHANGE",\
    "CMYTH_EVENT_DONE_RECORDING",\
    "CMYTH_EVENT_QUIT_LIVETV",\
    "CMYTH_EVENT_WATCH_LIVETV",\
    "CMYTH_EVENT_LIVETV_CHAIN_UPDATE",\
    "CMYTH_EVENT_SIGNAL",\
    "CMYTH_EVENT_ASK_RECORDING",\
    "CMYTH_EVENT_SYSTEM_EVENT",\
    "CMYTH_EVENT_UPDATE_FILE_SIZE",\
    "CMYTH_EVENT_GENERATED_PIXMAP",\
    "CMYTH_EVENT_CLEAR_SETTINGS_CACHE"};
  cmyth_event_t myth_event;
  char databuf[2049];
  databuf[0]=0;
  timeval t;
  t.tv_sec=5;
  t.tv_usec=0;

  while(Running())
  {
    myth_event=CMYTH->EventGet(m_conn_t,databuf,2048);
    std::cout<<"EVENT ID: "<<events[myth_event]<<" EVENT databuf:"<<databuf<<std::endl;
    if(myth_event==CMYTH_EVENT_LIVETV_CHAIN_UPDATE)
    {
      if(!m_rec.IsNull())
      {
        bool retval=m_rec.LiveTVChainUpdate(CStdString(databuf));
        XBMC->Log(LOG_NOTICE,"%s: CHAIN_UPDATE: %i",__FUNCTION__,retval);
      }
      else
        XBMC->Log(LOG_NOTICE,"%s: CHAIN_UPDATE - No recorder",__FUNCTION__);

    }
    if(myth_event==CMYTH_EVENT_SIGNAL)
    {
      std::cout<<databuf<<std::endl;
    }
    databuf[0]=0;

  }
}

/*
 *              MythTimer
 */


MythTimer::MythTimer()
  :m_timer_t()
{}
  MythTimer::MythTimer(cmyth_timer_t cmyth_timer)
    :m_timer_t(new MythPointer<cmyth_timer_t>())
  {
    *m_timer_t=cmyth_timer;
  }

  int MythTimer::RecordID()
  {
    return CMYTH->TimerRecordid(*m_timer_t);
  }

  int MythTimer::ChanID()
  {
    return CMYTH->TimerChanid(*m_timer_t);
  }

  time_t MythTimer::StartTime()
  {
    return CMYTH->TimerStarttime(*m_timer_t);
  }

  time_t MythTimer::EndTime()
  {
    return CMYTH->TimerEndtime(*m_timer_t);
  }

  CStdString MythTimer::Title()
  {
    char * title=CMYTH->TimerTitle(*m_timer_t);
    CStdString retval(title);
    CMYTH->RefRelease(title);
    return retval;
  }

  CStdString MythTimer::Description()
   {
    char * description=CMYTH->TimerDescription(*m_timer_t);
    CStdString retval(description);
    CMYTH->RefRelease(description);
    return retval;
  }

  int MythTimer::Type()
  {
    return CMYTH->TimerType(*m_timer_t);
  }

  CStdString MythTimer::Category()
  {
    char * category=CMYTH->TimerCategory(*m_timer_t);
    CStdString retval(category);
    CMYTH->RefRelease(category);
    return retval;
  }

/*
*								MythDatabase
*/

MythDatabase::MythDatabase()
  :m_database_t()
{
}


MythDatabase::MythDatabase(CStdString server,CStdString database,CStdString user,CStdString password):
m_database_t(new MythPointerThreadSafe<cmyth_database_t>())
{
  char *cserver=_strdup(server.c_str());
  char *cdatabase=_strdup(database.c_str());
  char *cuser=_strdup(user.c_str());
  char *cpassword=_strdup(password.c_str());

  *m_database_t=(CMYTH->DatabaseInit(cserver,cdatabase,cuser,cpassword));
  free(cserver);
  free(cdatabase);
  free(cuser);
  free(cpassword);
}

std::vector<MythChannel> MythDatabase::ChannelList()
{
  std::vector<MythChannel> retval;
  m_database_t->Lock();
  cmyth_chanlist_t cChannels=CMYTH->MysqlGetChanlist(*m_database_t);
  m_database_t->Unlock();
  int nChannels=CMYTH->ChanlistGetCount(cChannels);
  for(int i=0;i<nChannels;i++)
  {
    cmyth_channel_t chan=CMYTH->ChanlistGetItem(cChannels,i);
    long channum=CMYTH->ChannelChannum(chan);
    retval.push_back(MythChannel(chan,1==CMYTH->MysqlIsRadio(*m_database_t,channum)));
  }
  CMYTH->RefRelease(cChannels);
  return retval;
}

std::vector<MythProgram> MythDatabase::GetGuide(time_t starttime, time_t endtime)
{
  MythProgram *programs=0;
  m_database_t->Lock();
  int len=CMYTH->MysqlGetGuide(*m_database_t,&programs,starttime,endtime);
  m_database_t->Unlock();
  if(len==0)
    return std::vector<MythProgram>();
  std::vector<MythProgram> retval(programs,programs+len);
  CMYTH->RefRelease(programs);
  return retval;
}

std::vector<MythTimer> MythDatabase::GetTimers()
{
  std::vector<MythTimer> retval;
  m_database_t->Lock();
  cmyth_timerlist_t timers=CMYTH->MysqlGetTimers(*m_database_t);
  m_database_t->Unlock();
  int nTimers=CMYTH->TimerlistGetCount(timers);
  for(int i=0;i<nTimers;i++)
  {
    cmyth_timer_t timer=CMYTH->TimerlistGetItem(timers,i);
    retval.push_back(MythTimer(timer));
  }
  CMYTH->RefRelease(timers);
  return retval;
}

int MythDatabase::AddTimer(int chanid,CStdString description, time_t starttime, time_t endtime,CStdString title)
{
  m_database_t->Lock();
  int retval=CMYTH->MysqlAddTimer(*m_database_t,chanid,description.Buffer(),starttime, endtime,title.Buffer());
  m_database_t->Unlock();
  return retval;
}

  bool MythDatabase::DeleteTimer(int recordid)
  {
  m_database_t->Lock();
  bool retval= CMYTH->MysqlDeleteTimer(*m_database_t,recordid)==0;
  m_database_t->Unlock();
  return retval;
  }

  bool MythDatabase::UpdateTimer(int recordid,int chanid,CStdString description, time_t starttime, time_t endtime,CStdString title)
  {
  m_database_t->Lock();
  bool retval = CMYTH->MysqlUpdateTimer(*m_database_t,recordid,chanid,description.Buffer(),starttime, endtime,title.Buffer())==0;
  m_database_t->Unlock();
  return retval;
  }

/*
*								MythChannel
*/
MythChannel::MythChannel(cmyth_channel_t cmyth_channel,bool isRadio)
  : m_channel_t(new MythPointer<cmyth_channel_t>()),m_radio(isRadio)
{
  *m_channel_t=(cmyth_channel);
}


MythChannel::MythChannel()
  : m_channel_t(),m_radio(false)
{
}

int  MythChannel::ID()
{
  return CMYTH->ChannelChanid(*m_channel_t);
}

int  MythChannel::Number()
{
  return CMYTH->ChannelChannum(*m_channel_t);
}

CStdString  MythChannel::Name()
{
  char* cChan=CMYTH->ChannelName(*m_channel_t);
  CStdString retval(cChan);
  CMYTH->RefRelease(cChan);
  return retval;
}

CStdString  MythChannel::Icon()
{
  char* cIcon=CMYTH->ChannelIcon(*m_channel_t);
  CStdString retval(cIcon);
  CMYTH->RefRelease(cIcon);
  return retval;
}

bool  MythChannel::Visible()
{
  return CMYTH->ChannelVisible(*m_channel_t)>0;
}

bool MythChannel::IsRadio()
{
  return m_radio;
}
/*
 *            MythProgramInfo
 */

MythProgramInfo::MythProgramInfo()
  :m_proginfo_t()
{
}

MythProgramInfo::MythProgramInfo(cmyth_proginfo_t cmyth_proginfo)
  :m_proginfo_t(new MythPointer<cmyth_proginfo_t>())
{
  *m_proginfo_t=cmyth_proginfo;
}

  CStdString MythProgramInfo::ProgramID()
  {
    CStdString retval;
    char* progId=CMYTH->ProginfoProgramid(*m_proginfo_t);
    retval=progId;
    CMYTH->RefRelease(progId);
    return retval;
  }

  CStdString MythProgramInfo::Title()
  {
    CStdString retval;
    char* title=CMYTH->ProginfoTitle(*m_proginfo_t);
    retval=title;
    CMYTH->RefRelease(title);
    return retval;
  }

  CStdString MythProgramInfo::Path()
   {
    CStdString retval;
    char* path=CMYTH->ProginfoPathname(*m_proginfo_t);
    retval=path;
    CMYTH->RefRelease(path);
    return retval;
  }

  CStdString MythProgramInfo::Description()
  {
    CStdString retval;
    char* desc=CMYTH->ProginfoDescription(*m_proginfo_t);
    retval=desc;
    CMYTH->RefRelease(desc);
    return retval;
  }

  CStdString MythProgramInfo::ChannelName()
    {
    CStdString retval;
    char* chan=CMYTH->ProginfoChanname(*m_proginfo_t);
    retval=chan;
    CMYTH->RefRelease(chan);
    return retval;
  }

  int  MythProgramInfo::ChannelID()
  {
    return CMYTH->ProginfoChanId(*m_proginfo_t);
  }

  time_t MythProgramInfo::RecStart()
  {
    time_t retval;
    MythTimestamp time=CMYTH->ProginfoRecStart(*m_proginfo_t);
    retval=time.UnixTime();
    return retval;
  }

  int MythProgramInfo::Duration()
  {
    return CMYTH->ProginfoLengthSec(*m_proginfo_t);
  }

  CStdString MythProgramInfo::Category()
  {
    CStdString retval;
    char* cat=CMYTH->ProginfoCategory(*m_proginfo_t);
    retval=cat;
    CMYTH->RefRelease(cat);
    return retval;
    }

  long long MythProgramInfo::uid()
     {
       long long retval=RecStart();
       retval<<=32;
       retval+=ChannelID();
       if(retval>0)
         retval=-retval;
       return retval;
     }
/*
 *            MythTimestamp
 */


MythTimestamp::MythTimestamp()
  :m_timestamp_t()
{
}

MythTimestamp::MythTimestamp(cmyth_timestamp_t cmyth_timestamp)
  :m_timestamp_t(new MythPointer<cmyth_timestamp_t>())
{
  *m_timestamp_t=(cmyth_timestamp);
}

  MythTimestamp::MythTimestamp(CStdString time,bool datetime)
 :m_timestamp_t(new MythPointer<cmyth_timestamp_t>())
{
  *m_timestamp_t=(/*datetime?CMYTH->DatetimeFromString(time.Buffer()):*/CMYTH->TimestampFromString(time.Buffer()));
}
 
  MythTimestamp::MythTimestamp(time_t time)
   :m_timestamp_t(new MythPointer<cmyth_timestamp_t>())
{
  *m_timestamp_t=(CMYTH->TimestampFromUnixtime(time));
}

  bool MythTimestamp::operator==(const MythTimestamp &other)
  {
    return CMYTH->TimestampCompare(*m_timestamp_t,*other.m_timestamp_t)==0;
  }

  bool MythTimestamp::operator>(const MythTimestamp &other)
  {
    return CMYTH->TimestampCompare(*m_timestamp_t,*other.m_timestamp_t)==1;
  }

  bool MythTimestamp::operator<(const MythTimestamp &other)
  {
    return CMYTH->TimestampCompare(*m_timestamp_t,*other.m_timestamp_t)==-1;
  }

  time_t MythTimestamp::UnixTime()
  {
    return CMYTH->TimestampToUnixtime(*m_timestamp_t);
  }

  CStdString MythTimestamp::String()
  {
    CStdString retval;
    char time[25];
    bool succeded=CMYTH->TimestampToString(time,*m_timestamp_t)==0;
    retval=succeded?time:"";
    return retval;
  }

  CStdString MythTimestamp::Isostring()
  {
    CStdString retval;
    char time[25];
    bool succeded=CMYTH->TimestampToIsostring(time,*m_timestamp_t)==0;
    retval=succeded?time:"";
    return retval;
  }

  CStdString MythTimestamp::Displaystring(bool use12hClock)
  {
    CStdString retval;
    char time[25];
    bool succeded=CMYTH->TimestampToDisplayString(time,*m_timestamp_t,use12hClock)==0;
    retval=succeded?time:"";
    return retval;
  }

/*   
*								MythConnection
*/

MythConnection::MythConnection():
m_conn_t(),m_server(""),m_port(0)
{  
}


MythConnection::MythConnection(CStdString server,unsigned short port):
m_conn_t(new MythPointer<cmyth_conn_t>),m_server(server),m_port(port)
{
  char *cserver=_strdup(server.c_str());
  cmyth_conn_t connection=CMYTH->ConnConnectCtrl(cserver,port,64*1024, 16*1024);
  free(cserver);
  *m_conn_t=(connection);
  
}

bool MythConnection::IsConnected()
{
  return *m_conn_t!=0;
}

MythRecorder MythConnection::GetFreeRecorder()
{
  return MythRecorder(CMYTH->ConnGetFreeRecorder(*m_conn_t));
}

  std::map<long long, MythProgramInfo>  MythConnection::GetRecordedPrograms()
  {
    std::map<long long, MythProgramInfo>  retval;
    cmyth_proglist_t proglist=CMYTH->ProglistGetAllRecorded(*m_conn_t);
    int len=CMYTH->ProglistGetCount(proglist);
    for(int i=0;i<len;i++)
    {
      MythProgramInfo prog=CMYTH->ProglistGetItem(proglist,i);
      retval.insert(std::pair<long long,MythProgramInfo>(prog.uid(),prog));
    }
    CMYTH->RefRelease(proglist);
    return retval;
  }

  std::map<long long, MythProgramInfo>  MythConnection::GetPendingPrograms()
   {
    std::map<long long, MythProgramInfo>  retval;
    cmyth_proglist_t proglist=CMYTH->ProglistGetAllPending(*m_conn_t);
    int len=CMYTH->ProglistGetCount(proglist);
    for(int i=0;i<len;i++)
    {
      MythProgramInfo prog=CMYTH->ProglistGetItem(proglist,i);
      retval.insert(std::pair<long long,MythProgramInfo>(prog.uid(),prog));
    }
    CMYTH->RefRelease(proglist);
    return retval;
  }

  std::map<long long, MythProgramInfo>  MythConnection::GetScheduledPrograms()
   {
    std::map<long long, MythProgramInfo>  retval;
    cmyth_proglist_t proglist=CMYTH->ProglistGetAllScheduled(*m_conn_t);
    int len=CMYTH->ProglistGetCount(proglist);
    for(int i=0;i<len;i++)
    {
      MythProgramInfo prog=CMYTH->ProglistGetItem(proglist,i);
      retval.insert(std::pair<long long,MythProgramInfo>(prog.uid(),prog));
    }
    CMYTH->RefRelease(proglist);
    return retval;
  }

  bool  MythConnection::DeleteRecording(MythProgramInfo &recording)
  {
    return CMYTH->ProginfoDeleteRecording(*m_conn_t,*recording.m_proginfo_t)==0;
  }


MythEventHandler MythConnection::CreateEventHandler()
{
  return MythEventHandler(m_server,m_port);
}

CStdString MythConnection::GetServer()
{
  return m_server;
}

int MythConnection::GetProtocolVersion()
{
  return CMYTH->ConnGetProtocolVersion(*m_conn_t);
}

bool MythConnection::GetDriveSpace(long long &total,long long &used)
{
  return CMYTH->ConnGetFreespace(*m_conn_t,&total,&used)==0;
}

bool MythConnection::UpdateSchedules(int id)
{
  CStdString cmd;
  cmd.Format("RESCHEDULE_RECORDINGS %i",id);
  return CMYTH->ScheduleRecording(*m_conn_t,cmd.Buffer())>=0;
  
}

MythFile MythConnection::ConnectFile(MythProgramInfo &recording)
{
  return CMYTH->ConnConnectFile(*recording.m_proginfo_t,*m_conn_t,64*1024, 16*1024);
}
/*
*								Myth Recorder
*/


MythRecorder::MythRecorder():
m_recorder_t(),livechainupdated()
{
}

MythRecorder::MythRecorder(cmyth_recorder_t cmyth_recorder):
m_recorder_t(new MythPointerThreadSafe<cmyth_recorder_t>()),livechainupdated(new int(0))
{
  *m_recorder_t=cmyth_recorder;
}

bool MythRecorder::SpawnLiveTV(MythChannel &channel)
{
  char* pErr=NULL;
  CStdString channelNum;
  channelNum.Format("%i",channel.Number());
  m_recorder_t->Lock();
  //check channel
  *livechainupdated=0;
  *m_recorder_t=(CMYTH->SpawnLiveTv(*m_recorder_t,64*1024, 16*1024,MythRecorder::prog_update_callback,&pErr,channelNum.GetBuffer()));
  int i=20;
  while(*livechainupdated==0&&i--!=0)
  {
    m_recorder_t->Unlock();
    Sleep(100);
    m_recorder_t->Lock();
  }
  m_recorder_t->Unlock();
  ASSERT(*m_recorder_t);
  
  MythProgramInfo info=CMYTH->RecorderGetCurProginfo(*m_recorder_t);

  if(pErr)
    std::cout<<__FUNCTION__<<pErr<<std::endl;
  return pErr==NULL;
}

bool MythRecorder::LiveTVChainUpdate(CStdString chainID)
{
  char* buffer=_strdup(chainID.c_str());
  m_recorder_t->Lock();
  bool retval=CMYTH->LivetvChainUpdate(*m_recorder_t,buffer,16*1024)==0;
  if(!retval)
    XBMC->Log(LOG_ERROR,"LiveTVChainUpdate failed on chainID: %s",buffer);
  *livechainupdated=1;
  m_recorder_t->Unlock();
  free(buffer);
  return retval;
}

void MythRecorder::prog_update_callback(cmyth_proginfo_t prog)
{
  std::cout<< "prog_update_callback";

}


bool MythRecorder::IsNull()
{
  return *m_recorder_t==NULL;
}



bool MythRecorder::IsRecording()
{
  m_recorder_t->Lock();
  bool retval=CMYTH->RecorderIsRecording(*m_recorder_t)==1;
  m_recorder_t->Unlock();
  return retval;
}

bool MythRecorder::CheckChannel(MythChannel &channel)
{
  m_recorder_t->Lock();
  CStdString channelNum;
  channelNum.Format("%i",channel.Number());
  bool retval=CMYTH->RecorderCheckChannel(*m_recorder_t,channelNum.GetBuffer())==1;
  m_recorder_t->Unlock();
  return retval;
}

bool MythRecorder::SetChannel(MythChannel &channel)
{
  m_recorder_t->Lock();
  //bool retval=CheckChannel(channel);
  bool retval=IsRecording();
  if(!retval)
    return retval;
  CStdString channelNum;
  channelNum.Format("%i",channel.Number());
  retval=CMYTH->RecorderPause(*m_recorder_t)==0;
  retval=CheckChannel(channel);
  retval=CMYTH->RecorderSetChannel(*m_recorder_t,channelNum.GetBuffer())==0;
  retval=CMYTH->LivetvChainSwitchLast(*m_recorder_t)==1;
  *livechainupdated=0;
  int i=20;
  while(*livechainupdated==0&&i--!=0)
  {
    m_recorder_t->Unlock();
    Sleep(100);
    m_recorder_t->Lock();
  }

  m_recorder_t->Unlock();
  for(int i=0;i<20;i++)
  {
    if(!IsRecording())
      Sleep(1);
    else
      break;
  }

  return retval;
}

int MythRecorder::ReadLiveTV(void* buffer,long long length)
{
  m_recorder_t->Lock();
  int bytesRead=CMYTH->LivetvRead(*m_recorder_t,static_cast<char*>(buffer),length);
  m_recorder_t->Unlock();
  return bytesRead;
}

MythProgramInfo MythRecorder::GetCurrentProgram()
{
  m_recorder_t->Lock();
  MythProgramInfo retval=CMYTH->RecorderGetCurProginfo(*m_recorder_t);
  m_recorder_t->Unlock();
  return retval;
}

long long MythRecorder::LiveTVSeek(long long offset, int whence)
{
  m_recorder_t->Lock();
  long long retval = CMYTH->LivetvSeek(*m_recorder_t,offset,whence);
  m_recorder_t->Unlock();
  return retval;
}

long long MythRecorder::LiveTVDuration()
{
  m_recorder_t->Lock();
  long long retval = CMYTH->LivetvChainDuration(*m_recorder_t);
  m_recorder_t->Unlock();
  return retval;
}

/*
 *        MythFile
 */


 MythFile::MythFile()
   :m_file_t()
 {

 }

  MythFile::MythFile(cmyth_file_t myth_file)
    : m_file_t(new MythPointer<cmyth_file_t>())
 {
   *m_file_t=myth_file;
 }

  bool  MythFile::IsNull()
  {
    return *m_file_t==0;
  }

  int MythFile::Read(void* buffer,long long length)
  {
   int bytesRead=CMYTH->FileRead(*m_file_t,static_cast<char*>(buffer),length);
   return bytesRead;
  }

  long long MythFile::Seek(long long offset, int whence)
  {
    return CMYTH->FileSeek(*m_file_t,offset,whence);
  }
  
  long long MythFile::Duration()
  {
    return CMYTH->FileLength(*m_file_t);
  }