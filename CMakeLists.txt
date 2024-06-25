set(_my_dir ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "")


function( add_shared_sources target)

target_sources(${target}  PRIVATE 
  ${_my_dir}/src/DistUtils.cpp
  ${_my_dir}/src/exceptions.cpp
  ${_my_dir}/src/Provenance.cpp
  ${_my_dir}/src/Utilities.cpp

  )

  target_include_directories(
    ${target}
    PRIVATE ${_my_dir}/include
    PUBLIC $<BUILD_INTERFACE:${_my_dir}/include>
           $<INSTALL_INTERFACE:include>)

endfunction()
