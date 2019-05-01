/*
 * CLARK, CLAssifier based on Reduced K-mers.
 */

/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   Copyright 2013-2016, Rachid Ounit <rouni001@cs.ucr.edu>
 */

/*
 * @author: Rachid Ounit, Ph.D Candidate.
 * @project: CLARK, Metagenomic and Genomic Sequences Classification project.
 * @note: C++ IMPLEMENTATION supported on latest Linux and Mac OS.
 *
 */

#include <cstdlib>
#include <string>
#include <vector>
#include <string.h>

#include "./file.hh"

void getElementsFromLine(std::string & line, const size_t& len, const int _maxElement, std::vector< std::string >& _elements)
{
	size_t t = 0; 
	size_t cpt = 0;
	_elements.resize(0);
	while (t < len && cpt < _maxElement)
	{
		while ( t < len  && (line[t] == ' ' || line[t] == '\t' || line[t] == '\n' || line[t] == '\r'))
		{       t++;
		}
		string v = "";
		while ( t < len && line[t] != ' '  && line[t] != '\t' && line[t] != '\n' && line[t] != '\r')
		{
			v.push_back(line[t]);
			t++;
		}
		if (v != "")
		{
			_elements.push_back(v);
			cpt++;
		}
	}
	return;
}

void getElementsFromLine(const std::string& line, const size_t& _maxElement, std::vector< std::string >& _elements)
{
	size_t t = 0, len = line.size();
	size_t cpt = 0;
	_elements.resize(0);
	while (t < len && cpt < _maxElement)
	{
		while (t <  len && (line[t] == ' ' || line[t] == ',' || line[t] == '\n' || line[t] == '\t' || line[t] == '\r'))
		{       t++;
		}
		string v = "";
		while (t <  len && line[t] != ' '  && line[t] != ',' && line[t] != '\n' && line[t] != '\t' && line[t] != '\r')
		{
			v.push_back(line[t]);
			t++;
		}
		if (v != "")
		{	
			_elements.push_back(v);
			cpt++;
		}
	}
	return;
}

void getElementsFromLine(const std::string& line, const vector<char>& _seps, std::vector< std::string >& _elements)
{
	size_t t = 0, len = line.size();
	size_t cpt = 0;
	_elements.resize(0);
	while (t < len)
	{
		bool checkSep = true;
		while (t < len && checkSep)
		{
			checkSep = false;
			for(size_t i = 0; i < _seps.size() && !checkSep;i++)
			{ checkSep =  line[t] == _seps[i];}
			t += checkSep ? 1:0;
		}
		string v = "";
		checkSep = true;
		while (checkSep && t < len)
		{
			for(size_t i = 0 ; i < _seps.size() && checkSep; i++)
			{
				checkSep = checkSep && line[t] != _seps[i];
			}
			if (checkSep)
			{
				v.push_back(line[t]);
				t++;
			}
		}
		if (v != "")
		{_elements.push_back(v);}
	}
	return;
}

bool getLineFromFile(ifstream& _fileStream, std::string& _line)
{
	std::string line;
	std::getline(_fileStream, line);
	if (line.length() != 0)
	{
		string l(line);
		if (l[l.size() - 1] == '\n')
		{
			l.erase(l.size() - 1, 1);
		}
		_line = l;
		return true;
	}
	else
	{
		_line = "";
		return false;
	}
	return false;
}
bool getFirstAndSecondElementInLine(ifstream& _fileStream, std::string& _line, ITYPE& _freq)
{
	std::string line;
	size_t len = 0;
	std::getline(_fileStream, line);
	len = line.length();
	if (len != 0)
	{
		// Take first element and put it into _line
		// Take second element and put it into _freq
		vector<string> ele;
		getElementsFromLine(line, len, 2, ele);
		_line = ele[0];
		_freq = atoi(ele[1].c_str());
		return true;
	}
	return false;
}
bool getFirstAndSecondElementInLine(ifstream& _fileStream, uint64_t& _kIndex, ITYPE& _index)
{
	std::string line;	
	size_t len = 0;
	std::getline(_fileStream, line);
	len = line.length();
	if (len != 0)
	{
		// Take the first element and put it into _kIndex: type IKMER
		// Take the second element and put it into _index: type ITYPE
		vector<string> ele;
		getElementsFromLine(line, len, 2, ele);

		_kIndex = atoll(ele[0].c_str());
		_index = atol(ele[1].c_str());
		return true;
	}
	return false;
}
bool getFirstElementInLineFromFile(ifstream& _fileStream, string& _line)
{
	std::string line;
	size_t len = 0;
	std::getline(_fileStream, line);
	len = line.length();
	if (len != 0)
	{
		vector<string> ele;
		getElementsFromLine(line, len, 1, ele);
		_line = ele[0];
		return true;
	}
	else
	{
		_line = "";
		return false;
	}
	return false;
}

void mergePairedFiles(const char* _file1, const char* _file2, const char* _objFile)
{
        string line1, line2 = "";
        vector<string> ele1;
        vector<string> ele2;
        vector<char> sep;
        sep.push_back(' ');
        sep.push_back('/');
        sep.push_back('\t');
		std::ifstream fd1;
		std::ifstream fd2;

		fd1.open(_file1);
		fd2.open(_file2);
        getLineFromFile(fd1, line1);
        getLineFromFile(fd2, line2);
        if (line1[0] != line2[0])
        {
                perror("Error: the files have different format!");
                exit(1);
        }
        char delim = line1[0];
        if (delim != '@')
        {
                perror("Error: paired-end reads must be FASTQ files!");
                exit(1);
        }
        sep.push_back(delim);

		fd1.clear();
		fd1.seekg(0);
		fd2.clear();
		fd2.seekg(0);

        ofstream fout(_objFile, std::ios::binary);
        while(getLineFromFile(fd1, line1) && getLineFromFile(fd2, line2))
        {
                if (line1[0] == delim && line2[0] == delim)
                {
                        ele1.clear();
                        ele2.clear();
                        getElementsFromLine(line1, sep, ele1);
                        getElementsFromLine(line2, sep, ele2);
                        if (ele1[0] != ele2[0])
                        {
                                perror("Error: read id does not match between files!");
                                exit(1);
                        }
                        fout << ">" << ele1[0] << endl;
                        if (getLineFromFile(fd1, line1) && getLineFromFile(fd2, line2))
                        {
                                // Add "N" to concatenate sequences, and separate content of each sequence
                                fout << line1 << "N" << line2 << endl;
                                if (getLineFromFile(fd1, line1) && getLineFromFile(fd2, line2))
                                {
                                        if (getLineFromFile(fd1, line1) && getLineFromFile(fd2, line2))
                                        {       continue;       }
                                }
                        }
                        else
                        {
                                perror("Error: Found read without sequence");
                                exit(1);
                        }
                        continue;
                }
        }
        fout.close();
}

void deleteFile(const char* _filename)
{
        if (_filename != NULL)
                remove(_filename);
}

bool validFile(const char* _file)
{
		std::ifstream fd;
		fd.open(_file);
        if (fd.fail())
        {       return false;   }
        return true;
}

bool validFolder(const char *_folder)
{
	struct stat info;

	if (stat(_folder, &info) != 0)
		return false;
	else if (info.st_mode & S_IFDIR)
		return true;
	else
		return false;
}

#ifdef WIN64
bool unmapFile(uint8_t* mapping, HANDLE handle)
{
	if (!mapping || handle == INVALID_HANDLE_VALUE)
		return false;

	//mapping handle also todo
	UnmapViewOfFile(mapping);
	CloseHandle(handle);

	return true;
}
bool mapExistingFileFromName(const char* name, uint8_t** pMap, HANDLE * handle)
{
	if (!pMap || !handle)
		return false;

	*pMap = 0;
	*handle = 0;

	HANDLE file = CreateFile(name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (file == INVALID_HANDLE_VALUE) 
	{ 
		printf("couldn't open %s\n", name); 
		return false;
	}
	*handle = file;

	HANDLE mapping = CreateFileMapping(file, 0, PAGE_READONLY, 0, 0, 0);
	if (mapping == 0) 
	{
		CloseHandle(file);
		printf("CreateFileMapping failed %s\n", name);
		return false; 
	}
	void* data = MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);
	
	if (!data)
	{
		CloseHandle(file);
		printf("MapViewOfFile failed %s\n", name);
		return false;
	}
	*pMap = (uint8_t*)data;

	return true;
}
#endif