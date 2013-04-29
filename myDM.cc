#include <stdio.h>
#include "myDM.hh"
#include "myPAGE.hh"

// pagess
Page *pages;

// Open data file and initialize tuple_list
myDM::myDM(int searchMode, int maxRecords, int numPages, string file):
	SEARCH_MODE(searchMode), MAX_NUM_RECORDS(maxRecords), NUM_PAGES(numPages), filename(file)
{
	// int(10 digits) + name(18) + phone(12) + (', 'x2)
	tuple_length = 44;
	// Load tuples into tuple_list
	loadTuples();
	
		// Open data file
	// datafile.open(filename.c_str(), fstream::in | fstream::out);
	// 	if (datafile.is_open()) {
	// 		struct tuple test_tuple = tuple_list[2];
	// 		test_tuple.ClientName.assign(" Clarice");
	// 		write(test_tuple);
	// 		read(1553);
	// 	}
	// 	
	// datafile.close();
}

/*** Data operation functions ***/

// Check if id exists - yes = 1 / no = -1 
int myDM::read(int id) {
	tr1::unordered_map<int,int>::const_iterator got = hash_index.find(id);
	// Not found
	if (got == hash_index.end()) {
		return -1;
	}
	// Found
	else {
		int num = hash_index[id]+1;
		goToLine(num);
		//@TODO -- not sure what should do here getting some extra stuff
		char * buffer = new char[30];
		datafile.read(buffer,30);
		//cout << "|" << buffer << "|\n";
		return 1;
	}
}

// Search for tuples with areaCode
int myDM::multRead(int areaCode) {
	tr1::unordered_map<int,int>::const_iterator it;
	for (it = hash_index.begin(); it != hash_index.end(); ++it) {
		//it->first it->second
		//cout << it->first << ":" << it->second << "\n";
	}
	return 1;
}

// Check if tuple exists - yes = overwrite / no = append 
int myDM::write(tuple newTuple) {
	// tuple_list[newTuple.ID] = newTuple;
	// hash_index[newTuple.ID] = newTuple.AreaCode;
	int num;
	tr1::unordered_map<int,int>::const_iterator got = hash_index.find(newTuple.ID);
	// Not found = append
	if (got == hash_index.end()) {
		num = next_line-1;
		datafile << "\n";
		next_line++;
	}
	// Found = overwrite
	else {
		num = hash_index[newTuple.ID]+1;
	}
	
	goToLine(num);
	stringstream ss;
	ss << newTuple.ID << "," << newTuple.ClientName << "," << newTuple.Phone;
	//cout << newTuple.Phone << "\n";
	string str = ss.str();
	str.append(tuple_length - str.length(), ' ');
	str.append(1, '\n');
	datafile << str;
	return 1;
}

// Delete filename -- basically clears all the tuples
int myDM::deleteData() {
	hash_index.clear();
	tuple_list.clear();
	
	datafile.seekg(0, ios::end);
	int size = teelg();
	datafile.seekg(0, ios::begin);
	
	for(int i = 0; i < size; i++)
		dataFile << '\0';
		
	//datafile.clear();
	
	return 1;
}

// Loads tuples into tuple_list and hash_index
void myDM::loadTuples() {
	string values; 

	fstream data;
	data.open(filename.c_str(), fstream::in | fstream::out);
	FILE * fixedfile = fopen("fixed.txt", "w");
	
	if (data.is_open()) {
		
		data.seekg(0, ios::end);
		
		int size = data.tellg();
		
		if(size == 0){
			cout << "size is 0" << endl;
			
			data.close();
			return;
			
		}
					
		values.reserve(size);
		data.seekg(0, std::ios::beg);
		
		values.assign((std::istreambuf_iterator<char>(data)), std::istreambuf_iterator<char>());
		
		istringstream lines(values.c_str());
		string line;
		
		data.close();
		
		int j = 0;
		struct tuple new_tuple;
		
		while(getline(lines, line)) {
						
			if(lines.str().compare("\n") != 0) {
				istringstream iss(line);
				string sub;
				string parsed_line[3];
				int i;
			
				// Assuming of format: 753, Seras, 482-626-6836 (', ' between each)
				for(i = 0; getline(iss,sub,','); i++) {
					parsed_line[i] = sub;
				}
						
				// Set new_tuple elements (ID, AreaCode, ClientName, Phone)
				new_tuple.ID = atoi(parsed_line[0].c_str());
				new_tuple.AreaCode = atoi(parsed_line[2].substr(1,3).c_str() );
			
				new_tuple.ClientName = string(parsed_line[1]);
				new_tuple.Phone = string(parsed_line[2]);
				
				//strncpy(new_tuple.Phone, parsed_line[2].c_str(), strlen(parsed_line[2].c_str()) );
			
				// cout << "ID:" << new_tuple.ID << "\n";
				// cout << "Area Code:" << new_tuple.AreaCode << "\n";
				// cout << "new_tuple.ClientName:" << new_tuple.ClientName << "\n";
				// cout << "parsed line:" << parsed_line[2].c_str() << "\n";
			
				// Insert into hash_index and tuple_list
				hash_index.insert(make_pair<int,int>(new_tuple.ID,j));
				tuple_list.insert(make_pair<int,struct tuple>(new_tuple.ID,new_tuple));
				j++;
			
				stringstream ss;
				ss << new_tuple.ID << "," << new_tuple.ClientName << "," << new_tuple.Phone;
				string str = ss.str();
				str.append(tuple_length - str.length(), ' ');
				str.append(1, '\n');
				fwrite(str.c_str(),sizeof(char),strlen(str.c_str()),fixedfile);
				ss.str("");
				ss.str().clear();
				str.clear();
			}
		}
		next_line = j;
		
	}
	else {
		cout << "Unable to open data file: " << filename << " in load()\n";
	}
	fclose(fixedfile);
	system("cp fixed.txt X.txt");
}



void myDM::writeOut() {
	ofstream newData;
	newData.open("X_new.txt");
	
	struct tuple curr;
	tr1::unordered_map<int,struct tuple>::const_iterator it;
	for (it = tuple_list.begin(); it != tuple_list.end(); ++it) {
		//it->first it->second
		curr = it->second;
		newData << curr.ID << "," << curr.ClientName << "," << curr.Phone << "\n";
	}
	newData.close();
}

void myDM::goToLine(int num){
		datafile.seekg(ios::beg);
    for(int i=0; i < num - 1; ++i){
        datafile.ignore(numeric_limits<streamsize>::max(),'\n');
    }
		datafile.seekp(datafile.tellg());
}