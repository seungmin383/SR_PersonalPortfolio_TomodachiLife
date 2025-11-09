#include "DataManager.h"

CDataManager::CDataManager()
{
}


struct ModelRow {
    std::string ProtoTag; // UTF-8
    bool isAnim;
};

// 2) 문자열 테이블 빌드
struct StringTableBuilder {
    std::unordered_map<std::string, uint32_t> map; // 문자열 -> 파일 기준 절대 오프셋
    std::string blob; // 0-종결로 이어붙임 (테이블 시작부터의 로컬 버퍼)

    // 반환: 테이블 시작(파일 기준) + 로컬 오프셋 = 절대 오프셋을 나중에 계산하므로
    // 여기선 로컬 오프셋을 리턴하고, 파일 쓸 때 절대 오프셋으로 바꿉니다.
    uint32_t AddOrGetLocalOffset(const std::string& s) {
        // map은 절대 오프셋을 넣을 거라 했지만, 여기선 로컬 오프셋으로 먼저 들고 있다가
        // 파일 배치가 정해진 뒤 절대 오프셋으로 변환해도 됩니다.
        // 단순화를 위해: map<string, uint32_t> 는 "로컬 오프셋"을 들고 있게 하겠습니다.
        auto it = map.find(s);
        if (it != map.end()) return it->second;

        uint32_t local = static_cast<uint32_t>(blob.size());
        blob.append(s);
        blob.push_back('\0');
        map.emplace(s, local);
        return local;
    }
};

#pragma pack(push, 1)
struct FileHeader {
    uint32_t Magic;              // 'MDLB' = 0x424C444D (리틀엔디언이면 'M''D''L''B'가 메모리에 거꾸로 보일 수 있음)
    uint32_t Version;            // 1
    uint32_t EntryCount;         // N
    uint32_t OffsetEntries;      // 파일 기준 오프셋
    uint32_t OffsetStringTable;  // 파일 기준 오프셋
};

struct Entry {
    uint32_t ProtoTagAbsOffset;  // 파일 기준 절대 오프셋
    uint8_t  Flags;              // bit0: isAnim
    uint8_t  Reserved[3]{};
};
#pragma pack(pop)

void CookJsonToBin(
    const std::string& jsonPath, 
    const std::string& binPath) 
{
    // Json 파일을 읽어와서 저장
    nlohmann::json ModelData;
    {
        std::ifstream fInput(jsonPath);
        if (!fInput) 
            throw std::runtime_error("JSON open failed");

        fInput >> ModelData;
    }

    std::vector<ModelRow> rows;
    for (auto& item : ModelData["Model"]) {
        ModelRow r;
        r.ProtoTag = item.value("ProtoTag", "");
        r.isAnim = item.value("isAnim", false);
        rows.push_back(std::move(r));
    }

    // (B) 문자열 테이블 빌드(로컬 오프셋 수집)
    StringTableBuilder stb;
    std::vector<uint32_t> protoTagLocalOffsets;
    protoTagLocalOffsets.reserve(rows.size());
    for (auto& r : rows) {
        protoTagLocalOffsets.push_back(stb.AddOrGetLocalOffset(r.ProtoTag));
    }

    // (C) 오프셋 배치 계산
    FileHeader hdr{};
    hdr.Magic = 'MDLB';   // 주의: 디버거에서 숫자값으로 보면 0x424C444D로 보일 수 있음
    hdr.Version = 1;
    hdr.EntryCount = static_cast<uint32_t>(rows.size());

    const uint32_t fileStart = 0;
    uint32_t offset = sizeof(FileHeader);

    // Entry 영역(4바이트 정렬)
    auto Align4 = [](uint32_t x) { return (x + 3u) & ~3u; };
    offset = Align4(offset);
    hdr.OffsetEntries = offset;
    uint32_t entriesSize = static_cast<uint32_t>(rows.size() * sizeof(Entry));
    offset += entriesSize;

    // StringTable (1바이트 정렬, 그냥 이어서 배치)
    hdr.OffsetStringTable = offset;
    uint32_t stringSize = static_cast<uint32_t>(stb.blob.size());
    offset += stringSize;

    // (D) 실제 Entry 작성(절대 오프셋으로 변환)
    std::vector<Entry> entries(rows.size());
    for (size_t i = 0; i < rows.size(); ++i) {
        Entry e{};
        const uint32_t local = protoTagLocalOffsets[i];
        const uint32_t absOffset = hdr.OffsetStringTable + local; // 파일 기준 절대
        e.ProtoTagAbsOffset = absOffset;
        e.Flags = static_cast<uint8_t>(rows[i].isAnim ? 0x1 : 0x0);
        entries[i] = e;
    }

    // (E) 파일 쓰기
    std::ofstream out(binPath, std::ios::binary);
    if (!out) throw std::runtime_error("BIN open failed");

    out.write(reinterpret_cast<const char*>(&hdr), sizeof(hdr));
    // 4바이트 정렬 패딩 필요 시 0 채움 (이번 레이아웃에선 이미 정렬 충족)
    out.write(reinterpret_cast<const char*>(entries.data()),
        static_cast<std::streamsize>(entries.size() * sizeof(Entry)));
    out.write(stb.blob.data(), static_cast<std::streamsize>(stb.blob.size()));
}





CDataManager* CDataManager::Create()
{
	return nullptr;
}

void CDataManager::Free()
{
	CBase::Free();
}
