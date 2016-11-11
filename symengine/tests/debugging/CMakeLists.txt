project(test_debug)

add_executable(cdict_cdict cdict_cdict.cpp)
target_link_libraries(cdict_cdict symengine catch)
add_test(cdict_cdict ${PROJECT_BINARY_DIR}/cdict_cdict)

add_executable(cdict_cdict2 cdict_cdict2.cpp)
target_link_libraries(cdict_cdict2 symengine catch)
add_test(cdict_cdict2 ${PROJECT_BINARY_DIR}/cdict_cdict2)

add_executable(cdict_cdefault cdict_cdefault.cpp)
target_link_libraries(cdict_cdefault symengine catch)
add_test(cdict_cdefault ${PROJECT_BINARY_DIR}/cdict_cdefault)

add_executable(cdict_cint cdict_cint.cpp)
target_link_libraries(cdict_cint symengine catch)
add_test(cdict_cint ${PROJECT_BINARY_DIR}/cdict_cint)

add_executable(cdict_cmap cdict_cmap.cpp)
target_link_libraries(cdict_cmap symengine catch)
add_test(cdict_cmap ${PROJECT_BINARY_DIR}/cdict_cmap)

add_executable(cdict_uintdict_map1 cdict_uintdict_map1.cpp)
target_link_libraries(cdict_uintdict_map1 symengine catch)
add_test(cdict_uintdict_map1 ${PROJECT_BINARY_DIR}/cdict_uintdict_map1)

add_executable(cmap_cmap cmap_cmap.cpp)
target_link_libraries(cmap_cmap symengine catch)
add_test(cmap_cmap ${PROJECT_BINARY_DIR}/cmap_cmap)

add_executable(cmap_cdict cmap_cdict.cpp)
target_link_libraries(cmap_cdict symengine catch)
add_test(cmap_cdict ${PROJECT_BINARY_DIR}/cmap_cdict)

add_executable(cmap_cint cmap_cint.cpp)
target_link_libraries(cmap_cint symengine catch)
add_test(cmap_cint ${PROJECT_BINARY_DIR}/cmap_cint)

add_executable(cmap_uintdict_map1 cmap_uintdict_map1.cpp)
target_link_libraries(cmap_uintdict_map1 symengine catch)
add_test(cmap_uintdict_map1 ${PROJECT_BINARY_DIR}/cmap_uintdict_map1)

add_executable(cint_cdict cint_cdict.cpp)
target_link_libraries(cint_cdict symengine catch)
add_test(cint_cdict ${PROJECT_BINARY_DIR}/cint_cdict)

add_executable(cint_cint cint_cint.cpp)
target_link_libraries(cint_cint symengine catch)
add_test(cint_cint ${PROJECT_BINARY_DIR}/cint_cint)

add_executable(cint_cmap cint_cmap.cpp)
target_link_libraries(cint_cmap symengine catch)
add_test(cint_cmap ${PROJECT_BINARY_DIR}/cint_cmap)

add_executable(cdict_dict_map1 cdict_dict_map1.cpp)
target_link_libraries(cdict_dict_map1 symengine catch)
add_test(cdict_dict_map1 ${PROJECT_BINARY_DIR}/cdict_dict_map1)

add_executable(cdict_dict_map2 cdict_dict_map2.cpp)
target_link_libraries(cdict_dict_map2 symengine catch)
add_test(cdict_dict_map2 ${PROJECT_BINARY_DIR}/cdict_dict_map2)

add_executable(cmap_dict_map1 cmap_dict_map1.cpp)
target_link_libraries(cmap_dict_map1 symengine catch)
add_test(cmap_dict_map1 ${PROJECT_BINARY_DIR}/cmap_dict_map1)

add_executable(cmap_dict_map2 cmap_dict_map2.cpp)
target_link_libraries(cmap_dict_map2 symengine catch)
add_test(cmap_dict_map2 ${PROJECT_BINARY_DIR}/cmap_dict_map2)