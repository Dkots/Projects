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
//--- Γείτονες του κόμβου ---//
struct vertex_neighbours {
	std::vector<int> vertex;
	std::vector<int> triangles;
	std::vector<math::vec> N_surface;
};
// Task 3
//--- Δυαδικό διάνυσμα ---//
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
//--- Κόμβος σωρού ---//
struct heap_node {
	int index_vec;
	double geodesic;
};
//--- Κλάση του σωρού ---//
class Heap {
public:
	/* Αρχικοποίηση του σωρού*/
	void HEAP_init(int i);
	/* Αφαίρεση ρίζας*/
	heap_node HEAP_pop();
	/* Ανακατάταξη κόμβων όταν αλλάζει τιμή ένας από αυτούς*/
	void change_data(int i, double x);
	/* Εκπτύπωση σωρού*/
	void HEAP_print();
	/* Αλλάγη τιμή των κόμβων*/
	void swap_heap_node(heap_node& dv1, heap_node& dv2);
	/* Μήκος σωρού*/
	int plithos;
	/* Διάνυσμα σωρού*/
	std::vector<heap_node> heap_D; // heap
};
//--- Κύρια κλάση ---//
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
	/* Υπολογίζει και αποθηκεύει τις καμπυλότητες σε αρχεία .txt του συνόλου εκπαίδευσης*/
	void load_dataset_curves();
	/* Υπολογίζει και αποθηκεύει τις καμπυλότητες σε αρχεία .txt του συνόλου δοκιμής*/
	void load_test_set_curves();
	/* Ξεχωρίζει και αποθηκεύει τις φυσικοχημικές ιδιότητες σε αρχεία .txt του συνόλου εκπαίδευσης*/
	void load_physhiomedical_properties_dataset();
	/* Ξεχωρίζει και αποθηκεύει τις φυσικοχημικές ιδιότητες σε αρχεία .txt του συνόλου δοκιμής*/
	void load_physhiomedical_properties_test_set();
	/* Εύρεση μέγιστης και ελάχιστης τιμής σε ένα διάνυσμα*/
	std::vector<double> find_max_min_value(std::vector<double> VecList);
	/* Εκπτύπωση διανύσματος VectorXf*/
	void print_VectorXf(Eigen::VectorXf& v);
	/* Εκπτύπωση διανύσματος MatrixXf*/
	void print_MatrixXf(Eigen::MatrixXf& arr);
	/* Υπολογισμός του σφάλματος δύο διανυσμάτων*/
	float calc_error(Eigen::VectorXf& v1, Eigen::VectorXf& v2);
	/* Εύρεση της θέσης της μέγιστης τιμής*/
	int max_index(Eigen::VectorXf v);
	/* Εύρεση της θέσης της μικρότερης τιμής*/
	int min_index(Eigen::VectorXf v);
	/* Ορίζουμε τα διανύσματα ομάδων με τις σωστές ταξινομήσεις πρωτεΐνων*/
	Eigen::VectorXf build_c_correct(int teams[5], int size_c);
	/* Κανονικοποίηση διανύσματος VectorXf*/
	void normalize_VectorXf(Eigen::VectorXf& v);

	//---Task 1---//
	/* Αποθήκευση σε διανύσματα τις φυσικοχημικές ιδιότητες των πρωτεΐνων*/
	void open_physicochemical_properties(std::string file, std::vector<double>& m_properties_col_0, std::vector<double>& m_properties_col_1, std::vector<double>& m_properties_col_2);
	/* Εύρεση μέσης τιμής και τοποθέτησει στο αντίστοιχο τρίγωνο*/
	void find_median_value_triangle(std::vector<double>& properties_col, std::vector<double>& median_value_vector);
	/* Συναρτήσεις τοποθέτησης χρώματος για τις τρεις ιδιότητες*/
	void find_colour_face_0(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face);
	void find_colour_face_1(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face);
	void find_colour_face_2(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face);

	//---Task 2---//
	/* Συνάρτηση υπολογισμού καμπυλοτήτων*/
	void calc_gauss_mean_curve();
	/* Κατασκευή ενός διανύσματος της δομής vertex_neighbours*/
	void find_vertex_neighbours(vvr::Mesh m_model, std::vector<vertex_neighbours>& m_NeighboursList);
	/* Υπολογισμός καμπυλότητας gauss*/
	void calc_gauss_curve(vvr::Mesh m_model, std::vector<vertex_neighbours>& m_NeighboursList, std::vector<double>& K_gauss_curve);
	/* Υπολογισμός του normal διανυσμάτων σε μία επιφάνεια*/
	math::vec calc_normal_surface(math::vec u, math::vec ui, math::vec ui_n);
	/* Εύρεση των normal διανυσμάτων*/
	void find_Normal_vertices_surface(vvr::Mesh m_model, std::vector<vertex_neighbours>& m_NeighboursList);
	/* Υπολογισμός καμπυλότητας mean*/
	void calc_mean_curve(vvr::Mesh m_model, std::vector<vertex_neighbours>& m_NeighboursList, std::vector<double>& H_mean_curve);
	/* Συναρτήσεις τοποθέτησης χρώματος για τις καμπυλότητες*/
	void find_colour_face_gauss(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face);
	void find_colour_face_mean(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face);

	//---Task 3---//
	/* Εύρεση των διανυσμάτων του δυαδικού γράφου*/
	void find_dual_vertices();
	/* Εύρεση των γειτονικών τριγώνων*/
	void find_neigh_triangles();
	/* Έλεγχος αν είναι γειτονικά*/
	bool is_neighboors_triangles(vvr::Triangle& tri1, vvr::Triangle& tri2);
	/* Υπολογισμός του βαθμού προεξοχής*/
	void find_protrusion_degree();
	/* Αλγόριθμος Dijkstra*/
	void dijkstra(int i);
	/*Αλγόριθμος N-rings*/
	void N_ring_algorithm();

	//---Task 4---//
	/* Προσθήκη του ιστουγράμματος του αντικειμένου σε έναν πίνακα MatrixXf*/
	void add_histObj_MatrixXf();
	/* Υπολογισμός του πίνακα ανομοιομορφίας*/
	void calc_dissimilarity_matrix(Eigen::MatrixXf& arr, Eigen::MatrixXf& diss_Matrix);
	/* Υπολογισμός του ιστογράμματος*/
	void calc_histogramm(double min_value, double max_value, std::vector<double>& data, Eigen::VectorXf& hist, std::string name);
	/* ’νοιγμα του αρχείου που περιέχει την καμπυλότητα του αντικειμένου και προσθήκηκ σε ένα διάνυσμα*/
	void open_curves(std::string file, std::vector<double>& K_curve);
	/* Προσθήκη του ιστουγράμματος του αντικειμένου σε έναν πίνακα MatrixXf*/
	void add_histProtein(int index, Eigen::MatrixXf& histogramms_Proteins);
	/* Υπολογισμός του πίνακα ανομοιομορφίας των πρωτεϊνών*/
	void calc_dissimilarity_matrix_combination(Eigen::MatrixXf& gauss, Eigen::MatrixXf& mean, Eigen::MatrixXf& diss_Matrix, std::string name, float pr_gauss);

	//---Task 5---//
	/* ’νοιγμα αρχείου που περιέχει τον πίνακα ανομοιομορφίας*/
	void open_dissimilarity_matrix(std::string file, Eigen::MatrixXf& diss_Matrix);
	/* Υπολογισμός του πίνακα ομοιομορφίας των πρωτεϊνών με συγκεκριμένο κατώφλι*/
	void similar_protein(float T, std::string name);
	/* Ταξινόμηση πρωτεϊνων*/
	float classification_protein(int teams[5], std::string name);

	//---Task 6---//
	/* ’νοιγμα αρχείου που περιέχει το ιστόγραμμα του αντικειμένου*/
	void open_hist(std::string name, Eigen::VectorXf& hist);
	/* ’νοιγμα αρχείου που περιέχει την ταξινόμηση των πρωτεϊνων σε ομάδες*/
	void open_class_weight_gauss(Eigen::VectorXf& c, std::string name);
	/* Εύρεση της πιο όμοιας πρωτεΐνης και προσθήκη σε διάνυσμα*/
	void find_similar_vector();
	/* Υπολογισμός ποσοστού ανάκτησης*/
	void retriaval_degree(int teams[5]);

	//---Task 7---//
	/* Υπολογισμός του πίνακα ανομοιομορφίας για τις φυσικοχημικές ιδιότητες*/
	void calc_dissimilarity_matrix_prop(Eigen::MatrixXf& arr_histogramms, Eigen::MatrixXf& diss_matrix, std::string name);
	/* Υπολογισμός του πίνακα ανομοιομορφίας για όλες τις φυσικοχημικές ιδιότητες*/
	void calc_dissimilarity_matrix_prop_total(std::string name);
	/* Υπολογισμός του πίνακα ομοιομορφίας των πρωτεϊνών με συγκεκριμένο κατώφλι για τις φυσικοχημικές ιδιότητες*/
	void similar_prop(float T, Eigen::MatrixXf& diss_matrix, std::string name);
	/* Ταξινόμηση πρωτεϊνων*/
	void classification_protein_prop(Eigen::MatrixXf& diss_matrix, Eigen::VectorXf& c);
	/* Εύρεση της πιο όμοιας πρωτεΐνης και προσθήκη σε διάνυσμα σύμφωνα με τις φυσικοχημικές ιδιότητες*/
	void find_similar_vector_prop(Eigen::VectorXf& Vec_similar, std::string propertie);
	/* Εύρεση της πιο όμοιας πρωτεΐνης και προσθήκη σε διάνυσμα για όλες τις φυσικοχημικές ιδιότητες*/
	void find_similar_vector_prop_total();
	/* Υπολογισμός ποσοστού ανάκτησης*/
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
	std::vector<double> m_properties_col_0; // Διάνυσμα φυσικοχημικής ιδιότητας 0 
	std::vector<double> median_value_col_0; // Διάνυσμα με τις τιμές του κάθε τριγώνου
	std::vector<vvr::Colour> colour_face_0; // Διάνυσμα με τα χρώματα του κάθε τριγώνου
	double max_value_col_0; // Μέγιστη τιμή της φυσικοχημικής ιδιότητας 0 
	double min_value_col_0; // Ελάχιστη τιμή της φυσικοχημικής ιδιότητας 0 
	// Prop 1
	std::vector<double> m_properties_col_1; // Διάνυσμα φυσικοχημικής ιδιότητας 1
	std::vector<double> median_value_col_1; // Διάνυσμα με τις τιμές του κάθε τριγώνου
	std::vector<vvr::Colour> colour_face_1; // Διάνυσμα με τα χρώματα του κάθε τριγώνου
	double max_value_col_1; // Μέγιστη τιμή της φυσικοχημικής ιδιότητας 1
	double min_value_col_1; // Ελάχιστη τιμή της φυσικοχημικής ιδιότητας 1 
	// Prop 2
	std::vector<double> m_properties_col_2; // Διάνυσμα φυσικοχημικής ιδιότητας 2
	std::vector<double> median_value_col_2; // Διάνυσμα με τις τιμές του κάθε τριγώνου
	std::vector<vvr::Colour> colour_face_2; // Διάνυσμα με τα χρώματα του κάθε τριγώνου
	double max_value_col_2; // Μέγιστη τιμή της φυσικοχημικής ιδιότητας 2
	double min_value_col_2; // Ελάχιστη τιμή της φυσικοχημικής ιδιότητας 2
	// Αντικείμενα με τις ιδιότητες της πρωτείνης 
	vvr::Mesh m_model_col_0, m_model_col_1, m_model_col_2;

	//---Task 2---//
	std::vector<vertex_neighbours> m_NeighboursList; // Διάνυσμα που περιέχει τους γειτόνους του κάθε διανύσματος
	// Gauss
	std::vector<double> K_gauss_curve; // Διάνυσμα καμπυλότητας gauss
	double max_value_K; // Μέγιστη τιμή καμπυλότητα gauss
	double min_value_K; // Ελάχιστη τιμή καμπυλότητα gauss
	std::vector<double> median_value_K; // Διάνυσμα με τις τιμές του κάθε τριγώνου
	std::vector<vvr::Colour> colour_K; // Διάνυσμα με τα χρώματα του κάθε τριγώνου
	// Mean
	std::vector<double> H_mean_curve; // Διάνυσμα καμπυλότητας mean
	double max_value_H; // Μέγιστη τιμή καμπυλότητα mean
	double min_value_H; // Ελάχιστη τιμή καμπυλότητα mean
	std::vector<double> median_value_H; // Διάνυσμα με τις τιμές του κάθε τριγώνου
	std::vector<vvr::Colour> colour_H; // Διάνυσμα με τα χρώματα του κάθε τριγώνου
	// Αντικείμενα για αναπαράσταση
	vvr::Mesh m_model_gauss, m_model_mean;

	//---Task 3---//
	std::vector<dual_vertice> Graph; // Διάνυσμα με τα δυαδικά διανύσματα
	Heap heap_Dij; // Σωρός για τον αλγόριθμο Dijkstra
	double max_p; // Μέγιστη τιμή του βαθμού προεξοχής
	std::vector<int> is_projection; // Διάνυσμα αν είναι σημείο προεξοχής = 1

	//---Task 4---//
	//-- Ερώτημα i --//
	//- Αρμαντίλο -//
	vvr::Mesh m_model_1;
	std::vector<vertex_neighbours> m_NeighList_1;
	std::vector<double> K_1;
	//- Μονόκαιρος -//
	vvr::Mesh m_model_2;
	std::vector<vertex_neighbours> m_NeighList_2;
	std::vector<double> K_2;
	//- Κουνέλι -//
	vvr::Mesh m_model_3;
	std::vector<vertex_neighbours> m_NeighList_3;
	std::vector<double> K_3;
	// Μέγιστη και ελάχιστη καμπυλότητα
	double max_Gauss;
	double min_Gauss;
	double max_Mean;
	double min_Mean;
	// Ιστογράμματα
	Eigen::VectorXf hist_obj_1;
	Eigen::VectorXf hist_obj_2;
	Eigen::VectorXf hist_obj_3;
	Eigen::MatrixXf histogramm_Obj; // Πίνακας με τα τρία ιστογράμματα
	// Πίνακας ανομοιομορφίας για τα τρία αντικείμενα
	Eigen::MatrixXf dissimilarity_matrix;
	//-- Ερώτημα ii --//
	vvr::Mesh m_model_protein; // Μοντέλο πρωτεΐνης
	std::vector<vertex_neighbours> m_NeighList_protein;
	std::vector<double> K_protein;
	std::vector<double> H_protein;
	// Ιστογράμματα
	Eigen::VectorXf hist_obj_protein; // Βοηθητικό διάνυσμα
	Eigen::MatrixXf histogramms_Proteins_Gauss; // Τα ιστογράμματα πρωτεϊνων εκπαίδευσης καμπυλότητας gauss 
	Eigen::MatrixXf histogramms_Proteins_Mean; // Τα ιστογράμματα πρωτεϊνων εκπαίδευσης καμπυλότητας mean
	// Πίνακας ανομοιομορφίας σύμφωνα με τις καμπυλότητες
	Eigen::MatrixXf dissimilarity_matrix_proteins;

	//---Task 5---//
	// Πίνακας ομοιομορφίας σύμφωνα με τις καμπυλότητες
	Eigen::MatrixXf dissimilarity_matrix_proteins_bool;

	//---Task 6---//
	Eigen::VectorXf Vec_similar_protein; // Διάνυσμα με τις ομάδες που βρίσκονται οι πρωτεΐνες δοκιμής
	Eigen::VectorXf similar_test_data; // Βοηθητικό διάνυσμα
	// Βοηθητικά διανύσματα για την σύγκριση του συνόλου δοκιμής με το σύνολο εκπαίδευσης
	Eigen::VectorXf hist_obj_protein_dataset_gauss; 
	Eigen::VectorXf hist_obj_protein_dataset_mean;
	Eigen::VectorXf hist_obj_protein_testset_gauss;
	Eigen::VectorXf hist_obj_protein_testset_mean;

	//---Task 7---//
	// Διανύσματα με τις φυσικοχημικές ιδιότητες της πρωτεΐνης
	std::vector<double> protein_properties_col_0;
	std::vector<double> protein_properties_col_1;
	std::vector<double> protein_properties_col_2;
	// Μέγιστες και ελάχιστες τιμές
	double max_col_0;
	double min_col_0;
	double max_col_1;
	double min_col_1;
	double max_col_2;
	double min_col_2;
	// Ιστογράμματα
	Eigen::VectorXf hist_properties_col_0;
	Eigen::VectorXf hist_properties_col_1;
	Eigen::VectorXf hist_properties_col_2;
	// Πίνακες ιστογραμμάτων
	Eigen::MatrixXf arr_hist_properties_col_0;
	Eigen::MatrixXf arr_hist_properties_col_1;
	Eigen::MatrixXf arr_hist_properties_col_2;
	// task 4 //
	// Πίνακες ανομοιομορφίας
	Eigen::MatrixXf diss_properties_col_0;
	Eigen::MatrixXf diss_properties_col_1;
	Eigen::MatrixXf diss_properties_col_2;
	Eigen::MatrixXf diss_properties_total;
	// task 5
	// Πίνακας ομοιομορφίας
	Eigen::MatrixXf diss_matrix_prop_bool;
	// Ταξινόμηση των πρωτεΐνων σε ομάδες ανάλογα τις φυσικοχημικές ιδιότητες
	Eigen::VectorXf c_prop_0;
	Eigen::VectorXf c_prop_1;
	Eigen::VectorXf c_prop_2;
	Eigen::VectorXf c_prop_total;
	// task 6
	// Διανύσματα με τις ομάδες που βρίσκονται οι πρωτεΐνες δοκιμής
	Eigen::VectorXf Vec_similar_protein_col_0;
	Eigen::VectorXf Vec_similar_protein_col_1;
	Eigen::VectorXf Vec_similar_protein_col_2;
	Eigen::VectorXf Vec_similar_protein_col_total;
	Eigen::VectorXf similar_test_data_prop; // Βοηθητικό διάνυσμα
};