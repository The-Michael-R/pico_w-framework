find_package(Doxygen)
if (DOXYGEN_FOUND)

    message("Doxygen build started.")

    set(DOXYGEN_PROJECT_NAME ${PROJECT_NAME})
    set(DOXYGEN_PROJECT_BRIEF "Target board: ${PICO_BOARD}" )
    set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/docs)

    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "README.md")
    set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C "YES")
    set(DOXYGEN_AUTOLINK_SUPPORT "YES")
    set(DOXYGEN_TYPEDEF_HIDES_STRUCT "YES")
    set(DOXYGEN_EXTRACT_ALL "YES")
    set(DOXYGEN_EXTRACT_STATIC "YES")
    set(DOXYGEN_HIDE_SCOPE_NAMES "YES")
    set(DOXYGEN_RECURSIVE "YES")
    set(DOXYGEN_EXCLUDE_PATTERNS "_*.h")
    set(DOXYGEN_TIMESTAMP  "YES")
    set(DOXYGEN_GENERATE_DOCSET "NO")
    set(DOXYGEN_GENERATE_HTMLHELP "NO")
    set(DOXYGEN_DISABLE_INDEX "YES")
    set(DOXYGEN_GENERATE_TREEVIEW "YES")
    set(DOXYGEN_FULL_SIDEBAR "YES")

    set(DOXYGEN_NUM_PROC_THREADS 4)

    set(DOXYGEN_EXCLUDE "build")

    doxygen_add_docs(docs
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    COMMENT "Generating API documentation with Doxygen"
                    )
endif()
