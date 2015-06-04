#pragma once

#include "Common.h"
#include <math.h>
#include <vector>
#include <time.h>

class NSetCache
{
public:
	struct CacheLine
	{
		uint tag;
		uint index;
		uint offset;
	};
    
	struct CacheEntry
	{
		uint tag;
		uint data;
		bool isEmpty;
        
		CacheEntry() : tag(0), data(0), isEmpty(true) {}
		~CacheEntry() {}
	};
    
    struct CacheEntryGroup
    {
        time_t          timeStamp;
        CacheEntry*     entrys;
        bool            isRequiredUpdate;

        CacheEntryGroup() : timeStamp(0), entrys(nullptr) {}
        ~CacheEntryGroup() {}
    };
    

private:
	CacheLine                   _lineInfo;
	CacheEntryGroup**           _cacheDatas;
	uint						_nWay;
    uint                        _blockSize;
	uint						_cacheSetCount;
	uint						_hitCount;
	uint						_missCount;
    uint*                       _systemMemory;
    

public:
	NSetCache(uint cacheSize, uint cacheBlockSize, uint nWay, uint* systemMemory) : _nWay(nWay), _systemMemory(systemMemory), _blockSize(cacheBlockSize/4)
	{
		CacheLine line;
		line.offset = log(_blockSize);

		_cacheSetCount = (cacheSize / _blockSize) / nWay;
		line.index = log(_cacheSetCount);

		line.tag = 32 - (line.index - line.offset);

		_lineInfo = line;

        _cacheDatas = new CacheEntryGroup*[line.index];
        for(uint i=0; i<line.index; ++i)
        {
            _cacheDatas[i] = new CacheEntryGroup[nWay];
            for(uint j=0; j<nWay; ++j)
            {
                _cacheDatas[i][j].entrys = new CacheEntry[line.offset];
                for(uint k=0; k<line.offset; ++k)
                {
                    CacheEntry& entry = _cacheDatas[i][j].entrys[k];
                    entry.tag = 0;
                    entry.isEmpty = true;
                    entry.data = 0;
                }
                
                _cacheDatas[i][j].timeStamp = 0;
                _cacheDatas[i][j].isRequiredUpdate = false;
            }
        }
    }
    
	~NSetCache()
	{
	}

public:
	CacheLine MakeCacheLineCommand(uint address)
	{
		auto FillBit = [](unsigned int from, unsigned int to)
		{
			uint result = 0;

			for(uint i = from; i <= to; ++i)
				result |= (1 << (i + from));
			result <<= from;
			return result;
		};     
   
		uint offset = 32 - _lineInfo.tag;

		CacheLine line;
		line.tag = (address & FillBit(_lineInfo.tag, offset)) >> offset;
		line.index = (address & FillBit(_lineInfo.index, _lineInfo.offset)) >> _lineInfo.offset;
		line.offset = (address & FillBit(_lineInfo.offset, 0));

		return line;
	}

	bool IsValid(const CacheLine& command, CacheEntry** outEntry = nullptr, CacheEntryGroup** outEntryGroup = nullptr)
	{
		for(uint i=0; i<_nWay; ++i)
		{
            CacheEntry& entry = _cacheDatas[command.index][i].entrys[command.offset];
			if(entry.isEmpty == false)
            {
                if(outEntry)
                    (*outEntry) = &entry;
                if(outEntryGroup)
                    (*outEntryGroup) = &_cacheDatas[command.index][i];
				return true;
            }
		}

		return false;
	}
	bool IsTagMatch(const CacheLine& command, CacheEntry** outEntry = nullptr, CacheEntryGroup** outEntryGroup = nullptr)
	{
		for(uint i=0; i<_nWay; ++i)
		{
            CacheEntry& entry = _cacheDatas[command.index][i].entrys[command.offset];

			if(entry.tag == command.tag)
            {
                if(outEntry)
                    (*outEntry) = &entry;
                if(outEntryGroup)
                    (*outEntryGroup) = &_cacheDatas[command.index][i];
				return true;
            }
		}

		return false;
	}
    bool LoadCache(uint address)
    {
        CacheLine command = MakeCacheLineCommand(address);
        ASSERT_COND_MSG((address % 4) != 0, "Error, invalid memory address");
        uint offset = ((address / 4) / _blockSize) * _blockSize;

        bool success = false;
        for(uint wayIdx = 0; wayIdx < _nWay; ++wayIdx)
        {
            CacheEntry& firstEntry = _cacheDatas[command.index][wayIdx].entrys[0];
            if(firstEntry.isEmpty)
            {
                for(uint i=offset; i<offset + _blockSize; ++i)
                {
                    CacheEntry& entry = _cacheDatas[command.index][wayIdx].entrys[i - offset];

                    entry.data = _systemMemory[i];
                    entry.tag = command.tag;
                    entry.isEmpty = false;
                    success = true;
                }
                
                success = true;
            }

            if(success) break;
        }
        
        return success;
    }
    
    void Replace(uint address)
    {
        //maybe.., i think that write lru algo.
        CacheLine command = MakeCacheLineCommand(address);
        
        //first valus is wayIdx
        std::vector<std::pair<uint, time_t>> timeStamps;
        for(uint wayIdx = 0; wayIdx < _nWay; ++wayIdx)
        {
            timeStamps.push_back(std::make_pair(wayIdx, _cacheDatas[command.index][wayIdx].timeStamp));
        }
        
        auto sortFunc = [](const std::pair<uint, time_t>& left, std::pair<uint, time_t>& right)
        {
            return left.second < right.second;
        };
        
        std::sort(timeStamps.begin(), timeStamps.end(), sortFunc);
        const std::pair<uint, time_t>& replaceTarget = timeStamps[0];
        
		CacheEntryGroup& group = _cacheDatas[command.index][replaceTarget.first];

		if(group.isRequiredUpdate)
		{
			for(uint i=0; i<_blockSize; ++i)
			{
				CacheEntry& entry = group.entrys[i];

			}

			group.timeStamp = 0;
			group.isRequiredUpdate = false;
		}

        uint offset = ((address / 4) / _blockSize) * _blockSize;
        for(uint i=offset; i<offset + _blockSize; ++i)
        {			
            CacheEntry& entry = group.entrys[i - offset];
            
            entry.data = _systemMemory[i];
            entry.tag = command.tag;
            entry.isEmpty = false;
        }
    }
    
	uint FetchData(uint address)
	{
        uint result = 0;
		CacheLine command = MakeCacheLineCommand(address);

        CacheEntryGroup* entryGroup = nullptr;
        CacheEntry* entry = nullptr;
		bool isHit = IsValid(command) && IsTagMatch(command, &entry, &entryGroup);
		if(isHit) //딱히 더 손볼건 없는듯
        {
			_hitCount++;
            ASSERT_COND_MSG(entry, "Error, what the");
            result = entry->data;
        }
		else
		{
			_missCount++;
            
            if( LoadCache(address) == false )
                Replace(address);
            
            isHit = IsValid(command) && IsTagMatch(command, &entry, &entryGroup);
            ASSERT_COND_MSG(isHit, "Error, what the hell");
            result = entry->data;
		}
        
        entryGroup->timeStamp = time(nullptr);
        return result;
	}
    
    void InputData(uint address, uint data)
    {
        //캐시 쓰기 정책이란게 걸릴거야 좀.
        //흠 -_-; 쓰거나 접근할 때 timeStamp 업데이틑 해줬어? 아니. 아직
        //해볼까? ㅇㅇ 시작해보자
		//순서를 작성해보자면? 일단, 일단.. 캐시에 접근해봐

		CacheLine command = MakeCacheLineCommand(address);

		CacheEntryGroup* entryGroup = nullptr;
        CacheEntry* entry = nullptr;
		bool isHit = IsValid(command) && IsTagMatch(command, &entry, &entryGroup);
		//힛이라면, 해당 블럭만 변경해주면 될걸? 힛이 아니라면.. 검사를 좀 해봐야지
		if(isHit)
		{
			entry->data = data;
			entryGroup->isRequiredUpdate = true;
		}
		else
		{
			//힛이 안되면 메모리를 다시 부르긴 해야해. 변경할 때, 메모리 등록을 해주긴 해야하고
            if( LoadCache(address) == false )
                Replace(address);			
		}
    }
};