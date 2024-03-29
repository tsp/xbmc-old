#pragma once
/*
 *      Copyright (C) 2012 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

class CVariant;

namespace dbiplus
{
  class Dataset;
  class field_value;
}

typedef enum {
  // special fields used during sorting
  FieldNone = 0,
  FieldSort,        // used to store the string to use for sorting
  FieldSortSpecial, // whether the item needs special handling (0 = no, 1 = sort on top, 2 = sort on bottom)
  FieldLabel,
  FieldFolder,
  FieldMediaType,
  FieldRow,         // the row number in a dataset

  // special fields not retrieved from the database
  FieldSize,
  FieldDate,
  FieldDriveType,
  FieldStartOffset,
  FieldEndOffset,
  FieldProgramCount,
  FieldBitrate,
  FieldListeners,
  FieldPlaylist,
  FieldSet,
  FieldRandom,

  // fields retrievable from the database
  FieldId,
  FieldGenre,
  FieldAlbum,
  FieldArtist,
  FieldAlbumArtist,
  FieldTitle,
  FieldSortTitle,
  FieldYear,
  FieldTime,
  FieldTrackNumber,
  FieldFilename,
  FieldPath,
  FieldPlaycount,
  FieldLastPlayed,
  FieldInProgress,
  FieldRating,
  FieldComment,
  FieldDateAdded,
  FieldTvShowTitle,
  FieldPlot,
  FieldPlotOutline,
  FieldTagline,
  FieldTvShowStatus,
  FieldVotes,
  FieldDirector,
  FieldActor,
  FieldStudio,
  FieldCountry,
  FieldMPAA,
  FieldTop250,
  FieldNumberOfEpisodes,
  FieldNumberOfWatchedEpisodes,
  FieldWriter,
  FieldAirDate,
  FieldEpisodeNumber,
  FieldSeason,
  FieldEpisodeNumberSpecialSort,
  FieldSeasonSpecialSort,
  FieldReview,
  FieldThemes,
  FieldMoods,
  FieldStyles,
  FieldAlbumType,
  FieldMusicLabel,
  FieldTrailer,
  FieldVideoResolution,
  FieldVideoAspectRatio,
  FieldVideoCodec,
  FieldAudioChannels,
  FieldAudioCodec,
  FieldAudioLanguage,
  FieldSubtitleLanguage,
  FieldProductionCode,
  FieldChannelName
} Field;

typedef std::set<Field> Fields;
typedef std::vector<Field> FieldList;

typedef enum {
  MediaTypeNone = 0,
  MediaTypeMusic,
  MediaTypeArtist,
  MediaTypeAlbum,
  MediaTypeSong,
  MediaTypeVideo,
  MediaTypeVideoCollection,
  MediaTypeMusicVideo,
  MediaTypeMovie,
  MediaTypeTvShow,
  MediaTypeEpisode
} MediaType;

typedef enum {
  DatabaseQueryPartSelect,
  DatabaseQueryPartWhere,
  DatabaseQueryPartOrderBy,
} DatabaseQueryPart;

typedef std::map<Field, CVariant> DatabaseResult;
typedef std::vector<DatabaseResult> DatabaseResults;

class DatabaseUtils
{
public:
  static std::string MediaTypeToString(MediaType mediaType);
  static MediaType MediaTypeFromString(const std::string &strMediaType);

  static std::string GetField(Field field, MediaType mediaType, DatabaseQueryPart queryPart);
  static int GetFieldIndex(Field field, MediaType mediaType);
  static bool GetSelectFields(const Fields &fields, MediaType mediaType, FieldList &selectFields);
  
  static bool GetFieldValue(const dbiplus::field_value &fieldValue, CVariant &variantValue);
  static bool GetDatabaseResults(MediaType mediaType, const FieldList &fields, const std::auto_ptr<dbiplus::Dataset> &dataset, DatabaseResults &results);

  static std::string BuildLimitClause(int end, int start = 0);
};
