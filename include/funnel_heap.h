#include <cassert>
#include <cmath>
#include <type_traits>
#include <utility>
#include <cstdint>
#include <functional>
#include <vector>
#ifndef FUNNELHEAP
#define FUNNELHEAP


//#define funnel_wrong_test //не стоит расскоментировать:)
#define funnel_construct //default constructor
#define funnel_insert  // insert(T x)
#define funnel_get_min // getMin()
//#define funnel_make // make(T* array, size_t n)
#define funnel_extract_min // extractMin()
#define funnel_solyanka // ... , check const, reference, voids methods and
// constructors =&& && #define funnel_construct_comp //default constructor(comp)
#define funnel_insert_comp // insert(T x) with Compare
#define funnel_get_min_comp // getMin() with Compare
//#define funnel_make_comp // make(T* array, size_t n) with Compare
#define funnel_extract_min_comp // extractMin() with Compare
#define funnel_solyanka_comp // ... , check const, reference, voids methods
// and constructors =&& && with Compare 
//#define funnel_heap_
// //расскоментировать, когда куча будет готова

/* ------------------------Р—РђРњР•РќРРўР¬------------------------ */


const std::size_t B = 32;
std::size_t CurrentBlock = 0;

struct Address {
    std::size_t Address;
};

struct Block {
    std::vector<uint64_t> Data;
    Block() : Data(B) {}
};

Address Alloc() {
    Address result{CurrentBlock};
    CurrentBlock++;
    return result;
}

/* -------------------------------------------------------- */


//namespace FunnelHeap {
    struct BlockInfo {
        Address addr{};
        uint64_t MinElement{};
        uint64_t MaxElement{};
        std::size_t Count{};
    };

    // Min-РєСѓС‡Р°
    template<std::size_t InputBufferSize, typename ComparatorType = std::less<uint64_t>>
    class funnel_heap {
    private:
        std::vector<std::vector<BlockInfo>> Segments; // (0, 0) РЅРµ РёРјРµРµС‚ СЃРјС‹СЃР»Р°. Р’ РѕСЃС‚Р°Р»СЊРЅС‹С… СЏС‡РµР№РєР°С… РґР°РЅРЅС‹Рµ Рѕ Р±Р»РѕРєР°С….
        // Р‘РѕР»СЊС€РёРµ СЌР»РµРјРµРЅС‚С‹ Р±Р»РёР¶Рµ Рє РЅР°С‡Р°Р»Сѓ
        Block Root{}; // Р’РјРµСЃС‚Рѕ РЅР°С‡Р°Р»СЊРЅРѕРіРѕ Р±Р»РѕРєР°
        std::vector<uint64_t> ForInsert; // STL-РєСѓС‡Р° РґР»СЏ РЅРµРґР°РІРЅРѕ РґРѕР±Р°РІР»РµРЅРЅС‹С… СЌР»РµРјРµРЅС‚РѕРІ
        ComparatorType Comparator;
        std::vector<uint64_t>::iterator CurrentRoot = Root.Data.begin();
        std::vector<uint64_t>::iterator CurrentInsert = ForInsert.begin();

        // TODO
        void Insert() {}

        // TODO
        void Take() {}
    public:
        FunnelHeap() : ForInsert(B * InputBufferSize) {}

        explicit FunnelHeap(const ComparatorType & comp) : ForInsert(B * InputBufferSize), Comparator(comp) {}

        void insert(uint64_t value) {
            // Р•СЃР»Рё РІ РѕРїРµСЂР°С‚РёРІРЅРѕР№ РїР°РјСЏС‚Рё РµС‰Рµ РµСЃС‚СЊ РјРµСЃС‚Рѕ
            if (CurrentInsert != ForInsert.end()) {
                *CurrentInsert = value;
                CurrentInsert++;
                std::push_heap(ForInsert.begin(), CurrentInsert, Comparator);
                return;
            }
            CurrentInsert = ForInsert.begin();
            Insert();
        }

        void extractMin() {
            if (CurrentInsert == ForInsert.begin()) {
                assert(CurrentRoot != Root.Data.begin());
                CurrentRoot--;
                if (CurrentRoot == Root.Data.begin()) {
                    Take();
                }
                return;
            }
            if (CurrentRoot == Root.Data.begin()) {
                std::pop_heap(ForInsert.begin(), CurrentInsert, Comparator);
                CurrentInsert--;
                return;
            }
            auto result1 = CurrentRoot;
            result1--;
            auto result2 = CurrentInsert;
            result2--;
            if (*result1 > *result2) {
                std::pop_heap(ForInsert.begin(), CurrentInsert, Comparator);
                CurrentInsert--;
            } else {
                CurrentRoot--;
                if (CurrentRoot == Root.Data.begin()) {
                    Take();
                }
            }
        }

        [[nodiscard]] uint64_t getMin() {
            if (CurrentInsert == ForInsert.begin()) {
                assert(CurrentRoot != Root.Data.begin());
                auto result = CurrentRoot;
                result--;
                return *result;
            }
            if (CurrentRoot == Root.Data.begin()) {
                auto result = CurrentInsert;
                result--;
                return *result;
            }
            auto result1 = CurrentRoot;
            result1--;
            auto result2 = CurrentInsert;
            result2--;
            return (*result1 > *result2 ? *result2 : *result1);
        }

        ~FunnelHeap() = default;
    };

    // TODO: Merge
//}

#endif

