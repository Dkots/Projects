#include <VVRScene/canvas.h>
#include <VVRScene/mesh.h>
#include <VVRScene/settings.h>
#include <VVRScene/utils.h>
#include <MathGeoLib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <set>
#include <iomanip> 
#include <algorithm> 
#include <iterator> 
#include "symmetriceigensolver3x3.h"
#include "canvas.h"
#include "Eigen/Dense"
#include "Eigen/Sparse"
#include "Eigen/SparseQR"
#include "Eigen/Eigenvalues"
#include "Eigen/SparseCholesky"
#include "math.h"

#define FLAG_SHOW_AXES       1
#define FLAG_SHOW_WIRE       2
#define FLAG_SHOW_SOLID      4
#define FLAG_SHOW_NORMALS    8
#define FLAG_SHOW_PLANE     16
#define FLAG_SHOW_AABB      32

// Task 2
//--- Neighbors of the node ---//
struct vertex_neighbours {
	std::vector<int> vertex;
	std::vector<int> triangles;
	std::vector<math::vec> N_surface;
};
// Task 3
//--- Binary vector ---//
struct dual_vertice {
	math::vec dual_v;
	int index_vec;
	int count_neigh;
	int neigh_dual_vec[3] = { -1, -1, -1 };
	double distances[3] = { -1, -1, -1 };
	double area;
	double geodesic;
	double p = 0; // protrusion degree
	bool read;
};
//--- Heap node ---//
struct heap_node {
	int index_vec;
	double geodesic;
};
//--- Heap class ---//
class Heap {
public:
	/* Initializing the heap*/
	void HEAP_init(int i);
	/* Remove*/
	heap_node HEAP_pop();
	/* Reorder nodes when one of them changes value*/
	void change_data(int i, double x);
	/* Print*/
	void HEAP_print();
	/* Change Value*/
	void swap_heap_node(heap_node& dv1, heap_node& dv2);
	/* length heap*/
	int plithos;
	/* heap vector*/
	std::vector<heap_node> heap_D; // heap
};
//--- Main class ---//
class Mesh3DScene : public vvr::Scene
{
public:
	Mesh3DScene();
	const char* getName() const { return "3D Scene"; }
	void keyEvent(unsigned char key, bool up, int modif) override;
	void arrowEvent(vvr::ArrowDir dir, int modif) override;
	
private:
	void draw() override;
	void reset() override;
	void resize() override;
	void Tasks();

	//---Task 0---//
	/* Computes and saves the curvatures to .txt files of the training set*/
	void load_dataset_curves();
	/* Calculates and saves the curvatures to .txt files of the test set*/
	void load_test_set_curves();
	/* Extracts and saves the physicochemical properties to .txt files of the training set*/
	void load_physhiomedical_properties_dataset();
	/* Extracts and saves the physicochemical properties in .txt files of the test setò*/
	void load_physhiomedical_properties_test_set();
	/* Find the maximum and minimum value in a vector*/
	std::vector<double> find_max_min_value(std::vector<double> VecList);
	/* Print VectorXf vector*/
	void print_VectorXf(Eigen::VectorXf& v);
	/* Print MatrixXf vector*/
	void print_MatrixXf(Eigen::MatrixXf& arr);
	/* Calculation of the error of two vectors*/
	float calc_error(Eigen::VectorXf& v1, Eigen::VectorXf& v2);
	/* Finding the location of the maximum value*/
	int max_index(Eigen::VectorXf v);
	/* Finding the location of the maximum value*/
	int min_index(Eigen::VectorXf v);
	/* We define the group vectors with the correct protein classifications*/
	Eigen::VectorXf build_c_correct(int teams[5], int size_c);
	/* VectorXf normalization*/
	void normalize_VectorXf(Eigen::VectorXf& v);

	//---Task 1---//
	/* Storing in vectors the physicochemical properties of proteins*/
	void open_physicochemical_properties(std::string file, std::vector<double>& m_properties_col_0, std::vector<double>& m_properties_col_1, std::vector<double>& m_properties_col_2);
	/* Find the mean value and place it in the corresponding triangle*/
	void find_median_value_triangle(std::vector<double>& properties_col, std::vector<double>& median_value_vector);
	/* Color placement functions for the three properties*/
	void find_colour_face_0(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face);
	void find_colour_face_1(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face);
	void find_colour_face_2(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face);

	//---Task 2---//
	/* Curvature calculation function*/
	void calc_gauss_mean_curve();
	/* Construct a vector of the vertex_neighbors structure*/
	void find_vertex_neighbours(vvr::Mesh m_model, std::vector<vertex_neighbours>& m_NeighboursList);
	/* Calculate gauss curvature */
	void calc_gauss_curve(vvr::Mesh m_model, std::vector<vertex_neighbours>& m_NeighboursList, std::vector<double>& K_gauss_curve);
	/* Calculate normal vectors in one surface*/
	math::vec calc_normal_surface(math::vec u, math::vec ui, math::vec ui_n);
	/* Find normal vectors*/
	void find_Normal_vertices_surface(vvr::Mesh m_model, std::vector<vertex_neighbours>& m_NeighboursList);
	/* Calculate mean curvature*/
	void calc_mean_curve(vvr::Mesh m_model, std::vector<vertex_neighbours>& m_NeighboursList, std::vector<double>& H_mean_curve);
	/* Color placement functions for curvatures*/
	void find_colour_face_gauss(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face);
	void find_colour_face_mean(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face);

	//---Task 3---//
	/* Finding the vectors of the binary graph*/
	void find_dual_vertices();
	/* Finding adjacent triangles*/
	void find_neigh_triangles();
	/* Check if they are adjacent*/
	bool is_neighboors_triangles(vvr::Triangle& tri1, vvr::Triangle& tri2);
	/* Calculating the salience degree*/
	void find_protrusion_degree();
	/* Dijkstra Algorithm*/
	void dijkstra(int i);
	/*N-rings Algorithm*/
	void N_ring_algorithm();

	//---Task 4---//
	/* Adding the histogram of the object to a MatrixXf array*/
	void add_histObj_MatrixXf();
	/* Calculate dissimilarity matrix*/
	void calc_dissimilarity_matrix(Eigen::MatrixXf& arr, Eigen::MatrixXf& diss_Matrix);
	/* Calculation of the histogram*/
	void calc_histogramm(double min_value, double max_value, std::vector<double>& data, Eigen::VectorXf& hist, std::string name);
	/* Open the file containing the curvature of the object and addc to a vector*/
	void open_curves(std::string file, std::vector<double>& K_curve);
	/* Adding the histogram of the object to a MatrixXf array*/
	void add_histProtein(int index, Eigen::MatrixXf& histogramms_Proteins);
	/* Calculation of the protein dissimilarity matrix*/
	void calc_dissimilarity_matrix_combination(Eigen::MatrixXf& gauss, Eigen::MatrixXf& mean, Eigen::MatrixXf& diss_Matrix, std::string name, float pr_gauss);

	//---Task 5---//
	/* Open file containing disparity matrix*/
	void open_dissimilarity_matrix(std::string file, Eigen::MatrixXf& diss_Matrix);
	/* Calculation of protein similarity matrix with specific threshold*/
	void similar_protein(float T, std::string name);
	/* Protein classification*/
	float classification_protein(int teams[5], std::string name);

	//---Task 6---//
	/* Open a file containing the histogram of the object*/
	void open_hist(std::string name, Eigen::VectorXf& hist);
	/* Open a file containing the classification of proteins into groups*/
	void open_class_weight_gauss(Eigen::VectorXf& c, std::string name);
	/* Find the most similar protein and add to a vector*/
	void find_similar_vector();
	/* Recovery rate calculation*/
	void retriaval_degree(int teams[5]);

	//---Task 7---//
	/* Calculation of the disparity matrix for the physicochemical properties*/
	void calc_dissimilarity_matrix_prop(Eigen::MatrixXf& arr_histogramms, Eigen::MatrixXf& diss_matrix, std::string name);
	/* Calculation of the disparity matrix for all physicochemical properties*/
	void calc_dissimilarity_matrix_prop_total(std::string name);
	/* Calculation of the protein uniformity matrix with a certain threshold for the physicochemical properties*/
	void similar_prop(float T, Eigen::MatrixXf& diss_matrix, std::string name);
	/* Protein classification*/
	void classification_protein_prop(Eigen::MatrixXf& diss_matrix, Eigen::VectorXf& c);
	/* Finding the most similar protein and adding it to a vector according to the physicochemical properties*/
	void find_similar_vector_prop(Eigen::VectorXf& Vec_similar, std::string propertie);
	/* Finding the most similar protein and adding to a vector for all physicochemical properties*/
	void find_similar_vector_prop_total();
	/* Calculate retrival rate*/
	void retriaval_degree_prop(Eigen::VectorXf& Vec_similar, Eigen::VectorXf& c);

private:
	int m_style_flag;
	float m_plane_d;
	vvr::Canvas2D m_canvas;
	vvr::Colour m_obj_col;
	vvr::Mesh m_model_original, m_model;
	vvr::Box3D m_aabb;
	math::vec m_center_mass;
	math::vec m_pca_cen;
	math::vec m_pca_dir;
	math::Plane m_plane;
	std::vector<int> m_intersections;

	//---Task 1---//
	// Prop 0
	std::vector<double> m_properties_col_0; // Biological attribute vector 0 
	std::vector<double> median_value_col_0; // Vector with each value of triangle
	std::vector<vvr::Colour> colour_face_0; // Vector with the color of each triangle
	double max_value_col_0; // Max value 0 
	double min_value_col_0; // Min Value 0 
	// Prop 1
	std::vector<double> m_properties_col_1; // Biological attribute vector 1
	std::vector<double> median_value_col_1; // Vector with each value of triangle
	std::vector<vvr::Colour> colour_face_1; // Vector with the color of each triangle
	double max_value_col_1; // Max value 1
	double min_value_col_1; //  Min Value 1 
	// Prop 2
	std::vector<double> m_properties_col_2; // Biological attribute vector 2
	std::vector<double> median_value_col_2; // Vector with each value of triangle
	std::vector<vvr::Colour> colour_face_2; // Vector with the color of each triangle
	double max_value_col_2; // Max value 2
	double min_value_col_2; // Min Value 2
	//  Objects with the features of proteins
	vvr::Mesh m_model_col_0, m_model_col_1, m_model_col_2;

	//---Task 2---//
	std::vector<vertex_neighbours> m_NeighboursList; // Vector which involves the neighbors
	// Gauss
	std::vector<double> K_gauss_curve; // Vector gauss curvature
	double max_value_K; // Max gauss value
	double min_value_K; // Min gauss value
	std::vector<double> median_value_K; // Vector with each value of triangle 
	std::vector<vvr::Colour> colour_K; // Vector with the color of each triangle
	// Mean
	std::vector<double> H_mean_curve; // Vector mean curvature
	double max_value_H; // Max mean value
	double min_value_H; // Min mean value
	std::vector<double> median_value_H; // Vector with each value of triangle 
	std::vector<vvr::Colour> colour_H; // Vector with the color of each triangle
	// Showcasing objects
	vvr::Mesh m_model_gauss, m_model_mean;

	//---Task 3---//
	std::vector<dual_vertice> Graph; // Vector with dual_vertices
	Heap heap_Dij; // Heap for Dijkstra
	double max_p; // Max value
	std::vector<int> is_projection; // Vectro if is a projection point

	//---Task 4---//
	//-- Question i --//
	//- Armantilo -//
	vvr::Mesh m_model_1;
	std::vector<vertex_neighbours> m_NeighList_1;
	std::vector<double> K_1;
	//- Monokairos -//
	vvr::Mesh m_model_2;
	std::vector<vertex_neighbours> m_NeighList_2;
	std::vector<double> K_2;
	//- Kouneli -//
	vvr::Mesh m_model_3;
	std::vector<vertex_neighbours> m_NeighList_3;
	std::vector<double> K_3;
	// Max and min curvature
	double max_Gauss;
	double min_Gauss;
	double max_Mean;
	double min_Mean;
	// Histogramm
	Eigen::VectorXf hist_obj_1;
	Eigen::VectorXf hist_obj_2;
	Eigen::VectorXf hist_obj_3;
	Eigen::MatrixXf histogramm_Obj; // Matrix with three histogramm
	// Dissinilarity matrix of three objects 
	Eigen::MatrixXf dissimilarity_matrix;
	//-- Question ii --//
	vvr::Mesh m_model_protein; // Protein Model
	std::vector<vertex_neighbours> m_NeighList_protein;
	std::vector<double> K_protein;
	std::vector<double> H_protein;
	// Histogramms
	Eigen::VectorXf hist_obj_protein; // Default vector
	Eigen::MatrixXf histogramms_Proteins_Gauss; // Histogramms of training proteins gauss curvature 
	Eigen::MatrixXf histogramms_Proteins_Mean; // Histogramms of training proteins mean curvature 
	// Disiimilarity matrix of the proteins
	Eigen::MatrixXf dissimilarity_matrix_proteins;

	//---Task 5---//
	// Disiimilarity matrix of the proteins
	Eigen::MatrixXf dissimilarity_matrix_proteins_bool;

	//---Task 6---//
	Eigen::VectorXf Vec_similar_protein; // Vector which has the testing labels 
	Eigen::VectorXf similar_test_data; // Default Vector
	// Default Vectors
	Eigen::VectorXf hist_obj_protein_dataset_gauss; 
	Eigen::VectorXf hist_obj_protein_dataset_mean;
	Eigen::VectorXf hist_obj_protein_testset_gauss;
	Eigen::VectorXf hist_obj_protein_testset_mean;

	//---Task 7---//
	// Vectors with the biological attributes of proteins
	std::vector<double> protein_properties_col_0;
	std::vector<double> protein_properties_col_1;
	std::vector<double> protein_properties_col_2;
	// Max and Min Values
	double max_col_0;
	double min_col_0;
	double max_col_1;
	double min_col_1;
	double max_col_2;
	double min_col_2;
	// Histogramm
	Eigen::VectorXf hist_properties_col_0;
	Eigen::VectorXf hist_properties_col_1;
	Eigen::VectorXf hist_properties_col_2;
	// Matrix of histogramms
	Eigen::MatrixXf arr_hist_properties_col_0;
	Eigen::MatrixXf arr_hist_properties_col_1;
	Eigen::MatrixXf arr_hist_properties_col_2;
	// task 4 //
	// Disiimilarity matries
	Eigen::MatrixXf diss_properties_col_0;
	Eigen::MatrixXf diss_properties_col_1;
	Eigen::MatrixXf diss_properties_col_2;
	Eigen::MatrixXf diss_properties_total;
	// task 5
	// Disiimilarity matrix
	Eigen::MatrixXf diss_matrix_prop_bool;
	// Categorizes the proteins according to labels
	Eigen::VectorXf c_prop_0;
	Eigen::VectorXf c_prop_1;
	Eigen::VectorXf c_prop_2;
	Eigen::VectorXf c_prop_total;
	// task 6
	// Vectors which have the labels 
	Eigen::VectorXf Vec_similar_protein_col_0;
	Eigen::VectorXf Vec_similar_protein_col_1;
	Eigen::VectorXf Vec_similar_protein_col_2;
	Eigen::VectorXf Vec_similar_protein_col_total;
	Eigen::VectorXf similar_test_data_prop; // Default Vector
};