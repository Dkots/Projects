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
//--- �������� ��� ������ ---//
struct vertex_neighbours {
	std::vector<int> vertex;
	std::vector<int> triangles;
	std::vector<math::vec> N_surface;
};
// Task 3
//--- ������� �������� ---//
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
//--- ������ ����� ---//
struct heap_node {
	int index_vec;
	double geodesic;
};
//--- ����� ��� ����� ---//
class Heap {
public:
	/* ������������ ��� �����*/
	void HEAP_init(int i);
	/* �������� �����*/
	heap_node HEAP_pop();
	/* ����������� ������ ���� ������� ���� ���� ��� ������*/
	void change_data(int i, double x);
	/* ��������� �����*/
	void HEAP_print();
	/* ������ ���� ��� ������*/
	void swap_heap_node(heap_node& dv1, heap_node& dv2);
	/* ����� �����*/
	int plithos;
	/* �������� �����*/
	std::vector<heap_node> heap_D; // heap
};
//--- ����� ����� ---//
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
	/* ���������� ��� ���������� ��� ������������ �� ������ .txt ��� ������� �����������*/
	void load_dataset_curves();
	/* ���������� ��� ���������� ��� ������������ �� ������ .txt ��� ������� �������*/
	void load_test_set_curves();
	/* ��������� ��� ���������� ��� ������������� ��������� �� ������ .txt ��� ������� �����������*/
	void load_physhiomedical_properties_dataset();
	/* ��������� ��� ���������� ��� ������������� ��������� �� ������ .txt ��� ������� �������*/
	void load_physhiomedical_properties_test_set();
	/* ������ �������� ��� ��������� ����� �� ��� ��������*/
	std::vector<double> find_max_min_value(std::vector<double> VecList);
	/* ��������� ����������� VectorXf*/
	void print_VectorXf(Eigen::VectorXf& v);
	/* ��������� ����������� MatrixXf*/
	void print_MatrixXf(Eigen::MatrixXf& arr);
	/* ����������� ��� ��������� ��� �����������*/
	float calc_error(Eigen::VectorXf& v1, Eigen::VectorXf& v2);
	/* ������ ��� ����� ��� �������� �����*/
	int max_index(Eigen::VectorXf v);
	/* ������ ��� ����� ��� ���������� �����*/
	int min_index(Eigen::VectorXf v);
	/* �������� �� ���������� ������ �� ��� ������ ������������ ���������*/
	Eigen::VectorXf build_c_correct(int teams[5], int size_c);
	/* �������������� ����������� VectorXf*/
	void normalize_VectorXf(Eigen::VectorXf& v);

	//---Task 1---//
	/* ���������� �� ���������� ��� ������������� ��������� ��� ���������*/
	void open_physicochemical_properties(std::string file, std::vector<double>& m_properties_col_0, std::vector<double>& m_properties_col_1, std::vector<double>& m_properties_col_2);
	/* ������ ����� ����� ��� ����������� ��� ���������� �������*/
	void find_median_value_triangle(std::vector<double>& properties_col, std::vector<double>& median_value_vector);
	/* ����������� ����������� �������� ��� ��� ����� ���������*/
	void find_colour_face_0(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face);
	void find_colour_face_1(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face);
	void find_colour_face_2(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face);

	//---Task 2---//
	/* ��������� ����������� ������������*/
	void calc_gauss_mean_curve();
	/* ��������� ���� ����������� ��� ����� vertex_neighbours*/
	void find_vertex_neighbours(vvr::Mesh m_model, std::vector<vertex_neighbours>& m_NeighboursList);
	/* ����������� ������������ gauss*/
	void calc_gauss_curve(vvr::Mesh m_model, std::vector<vertex_neighbours>& m_NeighboursList, std::vector<double>& K_gauss_curve);
	/* ����������� ��� normal ����������� �� ��� ���������*/
	math::vec calc_normal_surface(math::vec u, math::vec ui, math::vec ui_n);
	/* ������ ��� normal �����������*/
	void find_Normal_vertices_surface(vvr::Mesh m_model, std::vector<vertex_neighbours>& m_NeighboursList);
	/* ����������� ������������ mean*/
	void calc_mean_curve(vvr::Mesh m_model, std::vector<vertex_neighbours>& m_NeighboursList, std::vector<double>& H_mean_curve);
	/* ����������� ����������� �������� ��� ��� ������������*/
	void find_colour_face_gauss(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face);
	void find_colour_face_mean(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face);

	//---Task 3---//
	/* ������ ��� ����������� ��� �������� ������*/
	void find_dual_vertices();
	/* ������ ��� ���������� ��������*/
	void find_neigh_triangles();
	/* ������� �� ����� ���������*/
	bool is_neighboors_triangles(vvr::Triangle& tri1, vvr::Triangle& tri2);
	/* ����������� ��� ������ ���������*/
	void find_protrusion_degree();
	/* ���������� Dijkstra*/
	void dijkstra(int i);
	/*���������� N-rings*/
	void N_ring_algorithm();

	//---Task 4---//
	/* �������� ��� �������������� ��� ������������ �� ���� ������ MatrixXf*/
	void add_histObj_MatrixXf();
	/* ����������� ��� ������ ��������������*/
	void calc_dissimilarity_matrix(Eigen::MatrixXf& arr, Eigen::MatrixXf& diss_Matrix);
	/* ����������� ��� �������������*/
	void calc_histogramm(double min_value, double max_value, std::vector<double>& data, Eigen::VectorXf& hist, std::string name);
	/* ������� ��� ������� ��� �������� ��� ����������� ��� ������������ ��� ��������� �� ��� ��������*/
	void open_curves(std::string file, std::vector<double>& K_curve);
	/* �������� ��� �������������� ��� ������������ �� ���� ������ MatrixXf*/
	void add_histProtein(int index, Eigen::MatrixXf& histogramms_Proteins);
	/* ����������� ��� ������ �������������� ��� ���������*/
	void calc_dissimilarity_matrix_combination(Eigen::MatrixXf& gauss, Eigen::MatrixXf& mean, Eigen::MatrixXf& diss_Matrix, std::string name, float pr_gauss);

	//---Task 5---//
	/* ������� ������� ��� �������� ��� ������ ��������������*/
	void open_dissimilarity_matrix(std::string file, Eigen::MatrixXf& diss_Matrix);
	/* ����������� ��� ������ ������������ ��� ��������� �� ������������ �������*/
	void similar_protein(float T, std::string name);
	/* ���������� ���������*/
	float classification_protein(int teams[5], std::string name);

	//---Task 6---//
	/* ������� ������� ��� �������� �� ���������� ��� ������������*/
	void open_hist(std::string name, Eigen::VectorXf& hist);
	/* ������� ������� ��� �������� ��� ���������� ��� ��������� �� ������*/
	void open_class_weight_gauss(Eigen::VectorXf& c, std::string name);
	/* ������ ��� ��� ������ ��������� ��� �������� �� ��������*/
	void find_similar_vector();
	/* ����������� �������� ���������*/
	void retriaval_degree(int teams[5]);

	//---Task 7---//
	/* ����������� ��� ������ �������������� ��� ��� ������������� ���������*/
	void calc_dissimilarity_matrix_prop(Eigen::MatrixXf& arr_histogramms, Eigen::MatrixXf& diss_matrix, std::string name);
	/* ����������� ��� ������ �������������� ��� ���� ��� ������������� ���������*/
	void calc_dissimilarity_matrix_prop_total(std::string name);
	/* ����������� ��� ������ ������������ ��� ��������� �� ������������ ������� ��� ��� ������������� ���������*/
	void similar_prop(float T, Eigen::MatrixXf& diss_matrix, std::string name);
	/* ���������� ���������*/
	void classification_protein_prop(Eigen::MatrixXf& diss_matrix, Eigen::VectorXf& c);
	/* ������ ��� ��� ������ ��������� ��� �������� �� �������� ������� �� ��� ������������� ���������*/
	void find_similar_vector_prop(Eigen::VectorXf& Vec_similar, std::string propertie);
	/* ������ ��� ��� ������ ��������� ��� �������� �� �������� ��� ���� ��� ������������� ���������*/
	void find_similar_vector_prop_total();
	/* ����������� �������� ���������*/
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
	std::vector<double> m_properties_col_0; // �������� ������������� ��������� 0 
	std::vector<double> median_value_col_0; // �������� �� ��� ����� ��� ���� ��������
	std::vector<vvr::Colour> colour_face_0; // �������� �� �� ������� ��� ���� ��������
	double max_value_col_0; // ������� ���� ��� ������������� ��������� 0 
	double min_value_col_0; // �������� ���� ��� ������������� ��������� 0 
	// Prop 1
	std::vector<double> m_properties_col_1; // �������� ������������� ��������� 1
	std::vector<double> median_value_col_1; // �������� �� ��� ����� ��� ���� ��������
	std::vector<vvr::Colour> colour_face_1; // �������� �� �� ������� ��� ���� ��������
	double max_value_col_1; // ������� ���� ��� ������������� ��������� 1
	double min_value_col_1; // �������� ���� ��� ������������� ��������� 1 
	// Prop 2
	std::vector<double> m_properties_col_2; // �������� ������������� ��������� 2
	std::vector<double> median_value_col_2; // �������� �� ��� ����� ��� ���� ��������
	std::vector<vvr::Colour> colour_face_2; // �������� �� �� ������� ��� ���� ��������
	double max_value_col_2; // ������� ���� ��� ������������� ��������� 2
	double min_value_col_2; // �������� ���� ��� ������������� ��������� 2
	// ����������� �� ��� ��������� ��� ��������� 
	vvr::Mesh m_model_col_0, m_model_col_1, m_model_col_2;

	//---Task 2---//
	std::vector<vertex_neighbours> m_NeighboursList; // �������� ��� �������� ���� ��������� ��� ���� �����������
	// Gauss
	std::vector<double> K_gauss_curve; // �������� ������������ gauss
	double max_value_K; // ������� ���� ����������� gauss
	double min_value_K; // �������� ���� ����������� gauss
	std::vector<double> median_value_K; // �������� �� ��� ����� ��� ���� ��������
	std::vector<vvr::Colour> colour_K; // �������� �� �� ������� ��� ���� ��������
	// Mean
	std::vector<double> H_mean_curve; // �������� ������������ mean
	double max_value_H; // ������� ���� ����������� mean
	double min_value_H; // �������� ���� ����������� mean
	std::vector<double> median_value_H; // �������� �� ��� ����� ��� ���� ��������
	std::vector<vvr::Colour> colour_H; // �������� �� �� ������� ��� ���� ��������
	// ����������� ��� ������������
	vvr::Mesh m_model_gauss, m_model_mean;

	//---Task 3---//
	std::vector<dual_vertice> Graph; // �������� �� �� ������� ����������
	Heap heap_Dij; // ����� ��� ��� ��������� Dijkstra
	double max_p; // ������� ���� ��� ������ ���������
	std::vector<int> is_projection; // �������� �� ����� ������ ��������� = 1

	//---Task 4---//
	//-- ������� i --//
	//- ��������� -//
	vvr::Mesh m_model_1;
	std::vector<vertex_neighbours> m_NeighList_1;
	std::vector<double> K_1;
	//- ���������� -//
	vvr::Mesh m_model_2;
	std::vector<vertex_neighbours> m_NeighList_2;
	std::vector<double> K_2;
	//- ������� -//
	vvr::Mesh m_model_3;
	std::vector<vertex_neighbours> m_NeighList_3;
	std::vector<double> K_3;
	// ������� ��� �������� �����������
	double max_Gauss;
	double min_Gauss;
	double max_Mean;
	double min_Mean;
	// ������������
	Eigen::VectorXf hist_obj_1;
	Eigen::VectorXf hist_obj_2;
	Eigen::VectorXf hist_obj_3;
	Eigen::MatrixXf histogramm_Obj; // ������� �� �� ���� ������������
	// ������� �������������� ��� �� ���� �����������
	Eigen::MatrixXf dissimilarity_matrix;
	//-- ������� ii --//
	vvr::Mesh m_model_protein; // ������� ���������
	std::vector<vertex_neighbours> m_NeighList_protein;
	std::vector<double> K_protein;
	std::vector<double> H_protein;
	// ������������
	Eigen::VectorXf hist_obj_protein; // ��������� ��������
	Eigen::MatrixXf histogramms_Proteins_Gauss; // �� ������������ ��������� ����������� ������������ gauss 
	Eigen::MatrixXf histogramms_Proteins_Mean; // �� ������������ ��������� ����������� ������������ mean
	// ������� �������������� ������� �� ��� ������������
	Eigen::MatrixXf dissimilarity_matrix_proteins;

	//---Task 5---//
	// ������� ������������ ������� �� ��� ������������
	Eigen::MatrixXf dissimilarity_matrix_proteins_bool;

	//---Task 6---//
	Eigen::VectorXf Vec_similar_protein; // �������� �� ��� ������ ��� ���������� �� ��������� �������
	Eigen::VectorXf similar_test_data; // ��������� ��������
	// ��������� ���������� ��� ��� �������� ��� ������� ������� �� �� ������ �����������
	Eigen::VectorXf hist_obj_protein_dataset_gauss; 
	Eigen::VectorXf hist_obj_protein_dataset_mean;
	Eigen::VectorXf hist_obj_protein_testset_gauss;
	Eigen::VectorXf hist_obj_protein_testset_mean;

	//---Task 7---//
	// ���������� �� ��� ������������� ��������� ��� ���������
	std::vector<double> protein_properties_col_0;
	std::vector<double> protein_properties_col_1;
	std::vector<double> protein_properties_col_2;
	// �������� ��� ��������� �����
	double max_col_0;
	double min_col_0;
	double max_col_1;
	double min_col_1;
	double max_col_2;
	double min_col_2;
	// ������������
	Eigen::VectorXf hist_properties_col_0;
	Eigen::VectorXf hist_properties_col_1;
	Eigen::VectorXf hist_properties_col_2;
	// ������� �������������
	Eigen::MatrixXf arr_hist_properties_col_0;
	Eigen::MatrixXf arr_hist_properties_col_1;
	Eigen::MatrixXf arr_hist_properties_col_2;
	// task 4 //
	// ������� ��������������
	Eigen::MatrixXf diss_properties_col_0;
	Eigen::MatrixXf diss_properties_col_1;
	Eigen::MatrixXf diss_properties_col_2;
	Eigen::MatrixXf diss_properties_total;
	// task 5
	// ������� ������������
	Eigen::MatrixXf diss_matrix_prop_bool;
	// ���������� ��� ��������� �� ������ ������� ��� ������������� ���������
	Eigen::VectorXf c_prop_0;
	Eigen::VectorXf c_prop_1;
	Eigen::VectorXf c_prop_2;
	Eigen::VectorXf c_prop_total;
	// task 6
	// ���������� �� ��� ������ ��� ���������� �� ��������� �������
	Eigen::VectorXf Vec_similar_protein_col_0;
	Eigen::VectorXf Vec_similar_protein_col_1;
	Eigen::VectorXf Vec_similar_protein_col_2;
	Eigen::VectorXf Vec_similar_protein_col_total;
	Eigen::VectorXf similar_test_data_prop; // ��������� ��������
};