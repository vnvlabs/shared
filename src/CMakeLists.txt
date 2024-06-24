
set(_my_dir ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "")

function( add_shared_includes target)
target_include_directories(${target}  PRIVATE ${_my_dir}/../include)
endfunction()

function( add_shared_sources target)

target_sources(${target}  PRIVATE 
  ${_my_dir}/DistUtils.cpp
  ${_my_dir}/exceptions.cpp
  ${_my_dir}/Provenance.cpp
  ${_my_dir}/Utilities.cpp

  )

  add_shared_includes(${target})

endfunction()
