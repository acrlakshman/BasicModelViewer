/*
 * This file is part of Basic Model Viewer (visualization package)
 *
 * Copyright (c) 2015, Lakshman Anumolu
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ParseEnSightPOV.h"

ParseEnSightPOV::ParseEnSightPOV()
{
}

ParseEnSightPOV::~ParseEnSightPOV()
{
}

void ParseEnSightPOV::TakeDown()
{
	this->vertex_positions.clear();
	this->vertex_normals.clear();
	this->vertex_indices.clear();
}

bool ParseEnSightPOV::LoadPOV(
	const char *file_name_
	)
{
	/* Load POV file into memory (CAUTION) */
	FILE *stream_f;
	stream_f = fopen(file_name_, "r");
	if (!stream_f) {
		perror("LoadPOV (fopen):");
		return false;
	}
	fseek(stream_f, 0, SEEK_END);

	long length_f = ftell(stream_f);
	//printf("length_f = %ld\n", length_f);

	rewind(stream_f);
	char buffer_[length_f];

	if (!fread(buffer_, sizeof(buffer_), 1, stream_f)) {
		perror("LoadPOV (fread):");
		return false;
	}
	buffer_[length_f] = '\0';

	/* Start parsing content in buffer_ */
	std::string buff_str_;
	int status_ = 0;
	bool is_buffer_end_ = false;
	for (int i = 0; !is_buffer_end_; ++i) {
		if ((buffer_[i] != '\n') && (buffer_[i] != '\0')) {
			buff_str_.push_back(buffer_[i]);
		} else {
			buff_str_.push_back('\0');
			// call string parser
			//printf("buff_str_ = %s\n", &buff_str_[0]);
			this->ParsePOVString(buff_str_, status_);
			buff_str_.clear();
			if (buffer_[i] == '\0')
				is_buffer_end_ = true;
		}
	}

	/*qDebug() << "this->vertex_positions.size() = " << this->vertex_positions.size();
	qDebug() << "vertex_positions[0] = " << this->vertex_positions[0];
	qDebug() << "vertex_positions[" << this->num_vertices-1 << "] = "
			<< this->vertex_positions[this->num_vertices-1];

	qDebug() << "this->vertex_normals.size() = " << this->vertex_normals.size();
	qDebug() << "vertex_normals[0] = " << this->vertex_normals[0];
	qDebug() << "vertex_normals[" << this->num_vertices-1 << "] = "
			<< this->vertex_normals[this->num_vertices-1];

	qDebug() << "this->vertex_indices.size() = " << this->vertex_indices.size();
	qDebug() << "vertex_indices[0] = " << this->vertex_indices[0];
	qDebug() << "vertex_indices[1] = " << this->vertex_indices[1];
	qDebug() << "vertex_indices[2] = " << this->vertex_indices[2];
	qDebug() << "vertex_indices[" << this->num_indices-3 << "] = "
			<< this->vertex_indices[this->num_indices-3];
	qDebug() << "vertex_indices[" << this->num_indices-2 << "] = "
			<< this->vertex_indices[this->num_indices-2];
	qDebug() << "vertex_indices[" << this->num_indices-1 << "] = "
			<< this->vertex_indices[this->num_indices-1];*/

	fclose(stream_f);
	return true;
}

bool ParseEnSightPOV::ParsePOVString(const std::string &str_, int &status_)
{
	/*
		status_ = 1 => found vertex_vectors
		status_ = 2 => recorded num_vertices
		status_ = 3 => end of num_vertices
		status_ = 4 => found normal_vectors
		status_ = 5 => read num_normal_vectors
		status_ = 6 => found face_indices
		status_ = 7 => recorded num_face_indices
	*/
	if (str_.find("vertex_vectors") != std::string::npos) {
		status_ = 1;
	} else if (str_.find("normal_vectors") != std::string::npos) {
		status_ = 4;
	} else if (str_.find("face_indices") != std::string::npos) {
		status_ = 6;
	} else if (str_.find("normal_indices") != std::string::npos) {
		status_ = 8;
	}

	if (status_ == 1 && str_.find("vertex_vectors") == std::string::npos) {
		int start_pos = -1;
		this->num_vertices = 0;

		if (!ExtractLastInt(str_, this->num_vertices, start_pos)) {
			qDebug() << "Error while reading number of vertices";
			return false;
		}

		status_ = 2;
	} else if (status_ == 2 && str_.find("}") == std::string::npos) {
		ExtractAllFloats(str_, status_);
	} else if (status_ == 4 && str_.find("normal_vectors") == std::string::npos) {
		int start_pos = -1;
		int num_normals = 0;
		if (!ExtractLastInt(str_, num_normals, start_pos)) {
			qDebug() << "Error while reading number of normal vectors";
			return false;
		}
		if (this->num_vertices != num_normals) {
			qDebug() << "number of vertex positions != number of normal vectors";
			return false;
		}

		status_ = 5;
	} else if (status_ == 5 && str_.find("}") == std::string::npos) {
		ExtractAllFloats(str_, status_);
	} else if (status_ == 6 && str_.find("face_indices") == std::string::npos) {
		int start_pos = -1;
		this->num_indices = 0;

		if (!ExtractLastInt(str_, this->num_indices, start_pos)) {
			qDebug() << "Error while reading number of face indices";
			return false;
		}
		this->num_indices *= 3;
		//qDebug() << "num_indices = " << this->num_indices;

		status_ = 7;
	} else if (status_ == 7 && str_.find("}") == std::string::npos) {
		ExtractAllInts(str_);
	}

	return true;
}

bool ParseEnSightPOV::ExtractLastInt(const std::string &str_, int &int_num_, int &start_pos)
{
	int end_pos = -1;
	std::string num_str;
	bool is_negative = false;
	for (int i = str_.size()-1; i >= 0; --i) {
		if ((str_[i] == ' ' || str_[i] == '\t' ||
			 str_[i] == '-' || str_[i] == '<' ||
			 str_[i] == '>' || str_[i] == ',') &&
			end_pos > 0 && start_pos < 0) {
			start_pos = i+1;
			if (str_[i] == '-')
				is_negative = true;
			break;
		}
		if (end_pos < 0 && (int)str_[i] >= 48 && (int)str_[i] <= 57) {
			end_pos = i;
		}
	}
	int num_digits_ = end_pos - start_pos + 1;

	for (int i = end_pos; i >= start_pos; --i) {
		num_str.insert(0, 1, str_[i]);
	}
	//printf("req_num_(int) = %d\n", is_negative ? -atoi(&num_str[0]) : atoi(&num_str[0]));
	if (end_pos < 0) {
		return false;
	} else {
		int_num_ = (is_negative ? -atoi(&num_str[0]) : atoi(&num_str[0]));
		return true;
	}
}

void ParseEnSightPOV::ExtractAllInts(const std::string &str_)
{
	std::string sub_str_ = str_.substr(0, str_.size());
	int int_num_ = 0.0;
	int start_pos = -1;

	QVector<unsigned int> face_indices_local_;

	while (ExtractLastInt(sub_str_, int_num_, start_pos)) {
		face_indices_local_.push_back(int_num_);

		sub_str_.clear();
		sub_str_ = str_.substr(0, start_pos);
		start_pos = -1;
	}

	if (face_indices_local_.size() == 3) {
		this->vertex_indices.push_back(face_indices_local_[2]-1);
		this->vertex_indices.push_back(face_indices_local_[1]-1);
		this->vertex_indices.push_back(face_indices_local_[0]-1);
	} else {
		/*printf("str_ = %s\n", &str_[0]);
		for (int i = face_indices_local_.size()-1; i >= 0; --i)
			qDebug() << "face_indices_local_[" << i << "]: " << face_indices_local_[i];
		qDebug() << "face_indices_local_ != 3";*/
	}

	face_indices_local_.clear();
}

bool ParseEnSightPOV::ExtractLastFloat(const std::string &str_, float &float_num_, int &start_pos)
{
	//printf("str_.size() = %d\n", str_.size());
	int end_pos = -1;
	std::string num_str;
	bool is_negative = false;
	for (int i = str_.size()-1; i >= 0; i--) {
		if ((str_[i] == ' ' || str_[i] == '\t' ||
			 str_[i] == '-' || str_[i] == '<' ||
			 str_[i] == '>' || str_[i] == ',') &&
			end_pos > 0 && start_pos < 0) {
			start_pos = i+1;
			if (str_[i] == '-')
				is_negative = true;
			break;
		}
		if (end_pos < 0 && (int)str_[i] >= 48 && (int)str_[i] <= 57) {
			end_pos = i;
		}
	}
	int num_digits_ = end_pos - start_pos + 1;

	if (end_pos >= 0) {
		for (int i = end_pos; i >= start_pos; --i) {
			num_str.insert(0, 1, str_[i]);
		}
		//printf("num_str = %s\n", &num_str[0]);
		//printf("req_num_(float) = %f\n", is_negative ? -atof(&num_str[0]) : atof(&num_str[0]));
	}

	if (end_pos < 0)
		return false;
	else {
		float_num_ = (is_negative ? -atof(&num_str[0]) : atof(&num_str[0]));
		return true;
	}
}

void ParseEnSightPOV::ExtractAllFloats(const std::string &str_, int &status_)
{
	std::string sub_str_ = str_.substr(0, str_.size());
	float float_num_ = 0.0;
	int start_pos = -1;

	QVector<float> floats_num_;

	while (ExtractLastFloat(sub_str_, float_num_, start_pos)) {
		floats_num_.push_back(float_num_);

		sub_str_.clear();
		sub_str_ = str_.substr(0, start_pos);
		start_pos = -1;
	}

	QVector3D vertex_attributes_local_;
	vertex_attributes_local_.setX(floats_num_[2]);
	vertex_attributes_local_.setY(floats_num_[1]);
	vertex_attributes_local_.setZ(floats_num_[0]);

	if (status_ == 2)
		this->vertex_positions.push_back(vertex_attributes_local_);
	else if (status_ == 5)
		this->vertex_normals.push_back(vertex_attributes_local_);

	floats_num_.clear();
}
