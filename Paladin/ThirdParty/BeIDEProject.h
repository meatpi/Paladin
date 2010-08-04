/*
	BeIDEProjectReader.cpp: a class for reading BeIDE projects
	©2010 DarkWyrm
	
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/
#ifndef BEIDEPROJECT_H
#define BEIDEPROJECT_H

// This is a class designed to read projects generated by the BeIDE environment.
// Based on reverse-engineering for compatibility, there are many parts of the
// file format not understood. It should, however, provide all of the useful
// information provided through the BeIDE interface and be sufficient for conversion
// to other build systems.

#include <DataIO.h>
#include <String.h>
#include <vector>

using std::vector;

enum
{
	FILE_TYPES_AUTODETECT = 0,
	FILE_TYPES_C_MODE,
	FILE_TYPES_CPP_MODE
};

enum
{
	TARGET_APPLICATION = 0,
	TARGET_SHARED_LIBRARY,
	TARGET_STATIC_LIBRARY,
	TARGET_KERNEL_DRIVER
};

enum
{
	LANGOPTS_ANSI_C_MODE			= 0x00000001,
	LANGOPTS_SUPPORT_TRIGRAPHS		= 0x00000010,
	LANGOPTS_SIGNED_CHAR			= 0x00000100,
	LANGOPTS_UNSIGNED_BITFIELDS		= 0x00001000,
	LANGOPTS_CONST_CHAR_LITERALS	= 0x00010000
};

enum
{
	WARNMODE_ENABLED = 0,
	WARNMODE_DISABLED,
	WARNMODE_AS_ERRORS
};

enum
{
	WARN_STRICT_ANSI 			= 0x00000001,
	WARN_LOCAL_SHADOW			= 0x00000002,
	WARN_INCOMPATIBLE_CAST		= 0x00000004,
	WARN_CAST_QUALIFIERS		= 0x00000008,
	WARN_CONFUSING_CAST			= 0x00000010,
	WARN_CANT_INLINE			= 0x00000020,
	WARN_EXTERN_TO_INLINE		= 0x00000040,
	WARN_OVERLOADED_VIRTUALS	= 0x00000080,
	WARN_C_CASTS				= 0x00000100,
	WARN_EFFECTIVE_CPP			= 0x00000200,
	
	WARN_MISSING_PARENTHESES	= 0x00001000,
	WARN_INCONSISTENT_RETURN	= 0x00002000,
	WARN_MISSING_ENUM_CASES		= 0x00004000,
	WARN_UNUSED_VARS			= 0x00008000,
	WARN_UNINIT_AUTO_VARS		= 0x00010000,
	WARN_INIT_REORDERING		= 0x00020000,
	WARN_NONVIRTUAL_DESTRUCTORS	= 0x00040000,
	WARN_UNRECOGNIZED_PRAGMAS	= 0x00080000,
	WARN_SIGNED_UNSIGNED_COMP	= 0x00100000,
	WARN_CHAR_SUBSCRIPTS		= 0x00200000,
	WARN_PRINTF_FORMATTING		= 0x00400000,
	WARN_TRIGRAPHS_USED			= 0x00800000,
	
	WARN_ALL_COMMON_ERRORS		= 0x00FFF000
};

enum
{
	CODEGEN_NO_PIC				= 0x00000001,
	CODEGEN_EXPLICIT_TEMPLATES	= 0x00000002,
	CODEGEN_IGNORE_INLINING		= 0x00000004,
	CODEGEN_PROFILING			= 0x00000008,
	CODEGEN_DEBUGGING			= 0x00000010,
	CODEGEN_OPTIMIZE_SIZE		= 0x00000020
};

enum
{
	OPTIMIZE_NONE = 0,
	OPTIMIZE_SOME,
	OPTIMIZE_MORE,
	OPTIMIZE_FULL
};

enum
{
	STRIP_ALL_SYMBOLS = 1,
	STRIP_ALL_LOCAL_SYMBOLS = 2
};

enum
{
	PATH_PROJECT_RELATIVE = 0,
	PATH_ABSOLUTE,
	PATH_IDE_RELATIVE
};

enum
{
	BUILD_IGNORE = 0,
	BUILD_PRECOMPILE,
	BUILD_COMPILE,
	BUILD_LINK,
	BUILD_POSTLINK
};

class ProjectFile
{
public:
	ProjectFile(void) {}
	
	BString	path;
	BString	mimeType;
	BString group;
};

class FileTypeRule
{
public:
	FileTypeRule(void) : hasResources(false) {}
	
	BString mimeType;
	BString extension;
	bool hasResources;
	BString toolName;
};


class BeIDEProject
{
public:
				BeIDEProject(const char *path);
				BeIDEProject(void);
				~BeIDEProject(void);
	
	status_t	InitCheck(void) const;
	status_t	SetTo(const char *path);
	void		Unset(void);
	
	void			SetTargetName(const char *name);
	const char *	TargetName(void) const;
	
	void			SetTargetType(const uint32 &type);
	uint32			TargetType(void) const;
	
	void			SetSystemIncludesAsLocal(bool value);
	bool			SystemIncludesAsLocal(void) const;
	
	// If file types are detected based on extension or defaulting to
	// C or C++
	void			SetFileDetectionMode(const uint32 &mode);
	uint32			FileDetectionMode(void) const;
	
	void			SetLanguageOptions(const uint32 &opts);
	uint32			LanguageOptions(void) const;
	
	// How warnings are treated -- enabled, disabled, or treated as errors
	void			SetWarningMode(const uint32 &mode);
	uint32			WarningMode(void) const;
	
	void			SetWarnings(const uint32 &warnFlags);
	uint32			Warnings(void) const;
	
	void			SetCodeGenerationFlags(const uint32 &codeFlags);
	uint32			CodeGenerationFlags(void) const;
	
	void			SetOptimizationMode(const uint32 &opMode);
	uint32			OptimizationMode(void) const;
	
	void			SetStripFlags(const uint32 &stripFlags);
	uint32			StripFlags(void) const;
	
	void			SetExtraCompilerOptions(const char *string);
	const char *	ExtraCompilerOptions(void);
	
	void			SetExtraLinkerOptions(const char *string);
	const char *	ExtraLinkerOptions(void);
	
	int32			CountSystemIncludes(void) const;
	const char *	SystemIncludeAt(const uint32 &index);
	
	int32			CountLocalIncludes(void) const;
	const char *	LocalIncludeAt(const uint32 &index);
	
	int32			CountFiles(void) const;
	ProjectFile		FileAt(const uint32 &index);
	
private:
	void		InitObject(void);
	
	// Convenience methods for working with file tags
	BString		TagIDToString(const uint32 &id);
	off_t		FindTagID(const int32 &id, const off_t &offset = 0);
	
	// The Read methods modify the offset, too, just like reading from a file
	int32		ReadInt32(off_t &offset);
	
	// Assumes that string is NULL-terminated
	BString		ReadString(off_t &offset);
	
	// Does all of the real work
	void		ParseData(void);
	
	status_t				fInit;
	uint8					*fBuffer;
	off_t					fBufferSize;
	
	// Project Data
	vector<ProjectFile>		fProjectFiles;
	BString					fTargetName;
	uint8					fTargetType;
	bool					fSystemIncludesAsLocal;
	uint8					fFileTypeMode;
	uint32					fLangOpts;
	uint8					fWarnMode;
	uint32					fWarnings;
	uint32					fCodeGenFlags;
	uint8					fOpMode;
	uint8					fStripFlags;
	BString					fCompilerOptions;
	BString					fLinkerOptions;
	
	// This is unused, but exists for completeness' sake
	vector<FileTypeRule>	fFileTypeRules;
	
	vector<BString>			fSysIncludes;
	vector<BString>			fLocalIncludes;
};

#endif
