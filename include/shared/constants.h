#include <string>

#ifndef VNV_CONST_TYPE_HEADER
#define VNV_CONST_TYPE_HEADER

namespace VnV {

namespace Constants {


namespace ActionStage {

typedef std::string type;
constexpr auto init = "i";
constexpr auto start = "s";
constexpr auto iter = "it";
constexpr auto end = "e";
constexpr auto final = "f";

};  // namespace ActionStage


namespace JSD {

constexpr auto outputFile = "outputFile";

#define NTYPES      \
  X(id, a)          \
  X(name, b)        \
  X(package, c)     \
  X(value, d)       \
  X(shape, e)       \
  X(node, f)        \
  X(meta, g)        \
  X(comm, h)        \
  X(children, i)    \
  X(prov, j)        \
  X(time, k)        \
  X(key, l)         \
  X(stage, m)       \
  X(info, n)        \
  X(message, o)     \
  X(internal, p)    \
  X(description, q) \
  X(result, r)      \
  X(stageId, s)     \
  X(mpiversion, t)  \
  X(input, u)       \
  X(reader, v)      \
  X(level, w)       \
  X(dtype, x)       \
  X(workflow, y)    \
  X(jobName, z)     \
  X(endid, aa)      \
  X(line, ab)       \
  X(filename, ac)   \
  X(results, ad)    \
  X(spec, ae)       \
  X(commList, af)   \
  X(testuid, ag)    \
  X(sdt, ah)        \
  X(title, ai)      \
  X(nodeMap, aj)    \
  X(worldSize, ak)  \
  X(date, al)       \
  X(filestub, am) 
#define X(A, b) constexpr auto A = #b;
NTYPES
#undef X
#undef NTYPES

}  // namespace JSD

namespace JSN {
#define NTYPES                    \
  X(log, a)                       \
  X(shape, b)                     \
  X(dataTypeStarted, c)           \
  X(dataTypeEnded, d)             \
  X(injectionPointStarted, e)     \
  X(injectionPointEnded, f)       \
  X(injectionPointIterStarted, g) \
  X(injectionPointIterEnded, h)   \
  X(packageOptionsStarted, i)     \
  X(packageOptionsFinished, j)    \
  X(actionStarted, k)             \
  X(actionFinished, l)            \
  X(workflowStarted, m)           \
  X(workflowFinished, n)          \
  X(workflowUpdated, o)           \
  X(fetch, p)                     \
  X(fetchSuccess, q)              \
  X(fetchFail, r)                 \
  X(testStarted, s)               \
  X(file, t)                      \
  X(done, u)                      \
  X(finalTime, v)                 \
  X(testFinished, w)              \
  X(unitTestStarted, x)           \
  X(unitTestFinished, y)          \
  X(commInfo, z)                  \
  X(info, aa)                     \
  X(initializationStarted, ab)    \
  X(initializationEnded, ac)      \
  X(stdout, ad)

#define X(A, b) constexpr auto A = #b;
NTYPES
#undef X
#undef NTYPES
}  // namespace JSN

namespace JST {
#define NTYPES X(Double, a) X(Long, b) X(String, c) X(Json, d) X(Bool, e) X(GlobalArray, f)
#define X(A, b) constexpr auto A = #b;
NTYPES
#undef X
#undef NTYPES
}  // namespace JST






}

}  // namespace VnV

#endif