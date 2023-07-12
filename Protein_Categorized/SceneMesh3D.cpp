#include "SceneMesh3D.h"

// 0: Προεπεξεργασία
// 1: Ερώτημα 1
// 2: Ερώτημα 2
// 3: Ερώτημα 3 ή 31 (Διαφορά στο draw())
// 41: Ερώτημα 4i
// 42: Ερώτημα 4ii
// 5: Ερώτημα 5
// 6: Ερώτημα 6
// 7: Ερώτημα 7
#define TASK 0
#define ON_FILE 1		// Αποθήκευση δεδομένων σε αρχεία
#define HIST_SIZE 50	// Αριθμός υποδιαστημάτων
#define WEIGHT_GAUSS 1  // Βάρος της καμπυλότητας gauss
#define T 0.002 		// Κατώφλι όμοιων πρωτεΐνων

// Συντομεύσεις
using namespace std;
using namespace vvr;
using namespace Eigen;

// Κατεύθυνση αρχείων
//---Task 0---//
const string objDirDataset = getBasePath() + "resources/dataset/surfaces/";
const string objDirTestset = getBasePath() + "resources/test_set/surfaces/";
const string objDirPropDataset = getBasePath() + "resources/dataset/properties/";
const string objDirPropTestset= getBasePath() + "resources/test_set/properties/";
//---Task 1---//
const string obj = "armadillo_low_low"; // Αριθμός Πρωτεΐνης
const string Set = "obj"; // Σύνολο
// Κατεύθυνση της πρωτεΐνης
const string objDirSurfaces = getBasePath() + "resources/" + Set + "/surfaces/";
const string objFileSurf = objDirSurfaces + obj +".obj";
// Κατεύθυνση των ιδιοτήτων
const string objDirProperties = getBasePath() + "resources/" + Set + "/properties/";
const string objFileProp = objDirProperties + obj +".txt";
//---Task 3---//
const string objDir = getBasePath() + "resources/" + Set + "/";
const string objFile = objDir + obj + ".obj";
//---Task 4I---//
const string objDir41 = getBasePath() + "resources/obj/";
// Αντικείμενο 1
const string objFile1 = objDir41 + "armadillo_low_low.obj";
// Αντικείμενο 2
const string objFile2 = objDir41 + "unicorn_low_low.obj";
// Αντικείμενο 3
const string objFile3 = objDir41 + "bunny_low.obj";
// Αποθήκευση Ιστογραμμάτων
const string hist_obj1 = getBasePath() + "resources/histogram/Task4i/Object1.txt";
const string hist_obj2 = getBasePath() + "resources/histogram/Task4i/Object2.txt";
const string hist_obj3 = getBasePath() + "resources/histogram/Task4i/Object3.txt";

Mesh3DScene::Mesh3DScene()
{
	//! Load settings.
	vvr::Shape::DEF_LINE_WIDTH = 4;
	vvr::Shape::DEF_POINT_SIZE = 10;
	m_perspective_proj = true;
	m_bg_col = Colour("768E77");
	m_obj_col = Colour("454545");
	m_model_original = vvr::Mesh(objFileSurf);
	reset();
}

void Mesh3DScene::reset()
{
	Scene::reset();
	//! Define what will be vissible by default
	m_style_flag = 0;
	m_style_flag |= FLAG_SHOW_SOLID;
	m_style_flag |= FLAG_SHOW_WIRE;
	m_style_flag |= FLAG_SHOW_AXES;
	m_style_flag |= FLAG_SHOW_AABB;
	m_style_flag |= FLAG_SHOW_PLANE;
}

void Mesh3DScene::resize()
{
	//! By Making `first_pass` static and initializing it to true,
	//! we make sure that the if block will be executed only once.

	static bool first_pass = true;

	if (first_pass)
	{
		m_model_original.setBigSize(getSceneWidth()/5);
		m_model_original.update();
		m_model = m_model_original;
		Tasks();
		first_pass = false;
	}
}

void Mesh3DScene::Tasks()
{
	// Ποεπεξεργασία
	if (TASK == 0) {
		cout << "load dataset curveture" << endl;
		load_dataset_curves();
		cout << "load test set curveture" << endl;
		load_test_set_curves();
		cout << "load dataset physhiomedical properties" << endl;
		load_physhiomedical_properties_dataset();
		cout << "load test set physhiomedical properties" << endl;
		load_physhiomedical_properties_test_set();
	}
	//---Task 1---//
	else if (TASK == 1) {
		// Τοποθέτηση ιδιοτήτων σε διανύσματα
		open_physicochemical_properties(objFileProp, m_properties_col_0, m_properties_col_1, m_properties_col_2);
		//// Εύρεση των τιμών και του χρώματος που θα πάρει το κάθε τρίγωνο 
		//propertie col 0
		vector<double> vd0 = find_max_min_value(m_properties_col_0);
		min_value_col_0 = vd0[0];
		max_value_col_0 = vd0[1];
		find_median_value_triangle(m_properties_col_0, median_value_col_0);
		find_colour_face_0(median_value_col_0, colour_face_0);
		// propertie col 1
		vector<double> vd1 = find_max_min_value(m_properties_col_1);
		min_value_col_1 = vd1[0];
		max_value_col_1 = vd1[1];
		find_median_value_triangle(m_properties_col_1, median_value_col_1);
		find_colour_face_1(median_value_col_1, colour_face_1);
		// propertie col 2
		vector<double> vd2 = find_max_min_value(m_properties_col_2);
		min_value_col_2 = vd2[0];
		max_value_col_2 = vd2[1];
		find_median_value_triangle(m_properties_col_2, median_value_col_2);
		find_colour_face_2(median_value_col_2, colour_face_2);
		//// Αναπαράσταση, αλλάγη θέσης των πρωτεινών
		for (int i = 0; i < m_model.getVertices().size(); i++) {
			vec v(-30, 0, 0);
			m_model.getVertices()[i] = m_model.getVertices()[i] + v;
		}
		m_model.update();
		// Prop 0
		m_model_col_0 = m_model_original;
		m_model_col_0.setBigSize(getSceneWidth() / 5);
		m_model_col_0.update();
		for (int i = 0; i < m_model_col_0.getVertices().size(); i++) {
			vec v(-10, 0, 0);
			m_model_col_0.getVertices()[i] = m_model_col_0.getVertices()[i] + v;
		}
		m_model_col_0.update();
		// Prop 1
		m_model_col_1 = m_model_original;
		m_model_col_1.setBigSize(getSceneWidth() / 5);
		m_model_col_1.update();
		for (int i = 0; i < m_model_col_1.getVertices().size(); i++) {
			vec v(10, 0, 0);
			m_model_col_1.getVertices()[i] = m_model_col_1.getVertices()[i] + v;
		}
		m_model_col_1.update();
		// Prop 2
		m_model_col_2 = m_model_original;
		m_model_col_2.setBigSize(getSceneWidth() / 5);
		m_model_col_2.update();
		for (int i = 0; i < m_model_col_2.getVertices().size(); i++) {
			vec v(30, 0, 0);
			m_model_col_2.getVertices()[i] = m_model_col_2.getVertices()[i] + v;
		}
		m_model_col_2.update();
	}
	//---Task 2---//
	else if (TASK == 2) {
		// Υπολογισμός gauss και mean καμπυλότητας
		calc_gauss_mean_curve();
		//// Αναπαράσταση, αλλάγη θέσης των πρωτεινών
		// Gauss
		m_model_gauss = m_model_original;
		m_model_gauss.setBigSize(getSceneWidth() / 2);
		m_model_gauss.update();
		for (int i = 0; i < m_model_gauss.getVertices().size(); i++) {
			vec v(-25, 0, 0);
			m_model_gauss.getVertices()[i] = m_model_gauss.getVertices()[i] + v;
		}
		m_model_gauss.update();
		// Mean
		m_model_mean = m_model_original;
		m_model_mean.setBigSize(getSceneWidth() / 2);
		m_model_mean.update();
		for (int i = 0; i < m_model_mean.getVertices().size(); i++) {
			vec v(25, 0, 0);
			m_model_mean.getVertices()[i] = m_model_mean.getVertices()[i] + v;
		}
		m_model_mean.update();
	}
	//---Task 3---//
	else if (TASK == 3 || TASK == 31) {
		find_dual_vertices();
		find_vertex_neighbours(m_model, m_NeighboursList);
		find_neigh_triangles();
		find_protrusion_degree();
		N_ring_algorithm();
		cout << "draw Graph" << endl;
	}
	//---Task 4I---//
	else if (TASK == 41) {
		hist_obj_1 = Eigen::VectorXf::Zero(HIST_SIZE);
		hist_obj_2 = Eigen::VectorXf::Zero(HIST_SIZE);
		hist_obj_3 = Eigen::VectorXf::Zero(HIST_SIZE);
		histogramm_Obj = Eigen::MatrixXf::Zero(3, HIST_SIZE);
		dissimilarity_matrix = Eigen::MatrixXf::Zero(3, 3);
		// Αντικείμενο 1
		cout << "Load object 1" << endl;
		m_model_1 = vvr::Mesh(objFile1);
		find_vertex_neighbours(m_model_1, m_NeighList_1);
		calc_gauss_curve(m_model_1, m_NeighList_1, K_1);
		vector<double> vd_K1 = find_max_min_value(K_1);
		double min_K_1 = vd_K1[0];
		double max_K_1 = vd_K1[1];
		min_Gauss = min_K_1;
		max_Gauss = max_K_1;
		// Αντικείμενο 2
		cout << "Load object 2" << endl;
		m_model_2 = vvr::Mesh(objFile2);
		find_vertex_neighbours(m_model_2, m_NeighList_2);
		calc_gauss_curve(m_model_2, m_NeighList_2, K_2);
		vector<double> vd_K2 = find_max_min_value(K_2);
		double min_K_2 = vd_K2[0];
		double max_K_2 = vd_K2[1];
		if (max_Gauss < max_K_2) {
			max_Gauss = max_K_2;
		}
		if (min_Gauss < min_K_2) {
			min_Gauss = min_K_2;
		}
		// Αντικείμενο 3
		cout << "Load object 3" << endl;
		m_model_3 = vvr::Mesh(objFile3);
		find_vertex_neighbours(m_model_3, m_NeighList_3);
		calc_gauss_curve(m_model_3, m_NeighList_3, K_3);
		vector<double> vd_K3 = find_max_min_value(K_3);
		double min_K_3 = vd_K3[0];
		double max_K_3 = vd_K3[1];
		if (max_Gauss < max_K_3) {
			max_Gauss = max_K_3;
		}
		if (min_Gauss < min_K_3) {
			min_Gauss = min_K_3;
		}
		// Κανονικοποίηση
		calc_histogramm(min_Gauss, max_Gauss, K_1, hist_obj_1, hist_obj1);
		cout << "Histogramm object 1" << endl;
		print_VectorXf(hist_obj_1);
		calc_histogramm(min_Gauss, max_Gauss, K_2, hist_obj_2, hist_obj1);
		cout << "Histogramm object 2" << endl;
		print_VectorXf(hist_obj_2);
		calc_histogramm(min_Gauss, max_Gauss, K_3, hist_obj_3, hist_obj1);
		cout << "Histogramm object 3" << endl;
		print_VectorXf(hist_obj_3);
		// Υπολογισμός dissimilarity matrix
		add_histObj_MatrixXf();
		calc_dissimilarity_matrix(histogramm_Obj, dissimilarity_matrix);
		cout << "Print dissimilarity matrix" << endl;
		print_MatrixXf(dissimilarity_matrix);
	}
	//---Task 4II---//
	else if (TASK == 42) {
		histogramms_Proteins_Gauss = MatrixXf::Zero(369, HIST_SIZE);
		histogramms_Proteins_Mean = MatrixXf::Zero(369, HIST_SIZE);
		dissimilarity_matrix_proteins = MatrixXf::Zero(369, 369);
		max_Gauss = 216.969;
		min_Gauss = -0.493732;
		max_Mean = 4.07601;
		min_Mean = 0.0257181;
		for (int i = 0; i < 369; i++) {
			string name = to_string(i) + ".txt";
			cout << name << endl;
			// Gauss
			hist_obj_protein = VectorXf::Zero(HIST_SIZE);
			string op = getBasePath() + "resources/curvature/dataset/Gauss/" + to_string(i) + ".txt";
			open_curves(op, K_protein);
			string hist = getBasePath() + "resources/histogram/curvature_dataset/Gauss/" + to_string(i) + ".txt";
			calc_histogramm(min_Gauss, max_Gauss, K_protein, hist_obj_protein, hist);
			K_protein.clear();
			add_histProtein(i, histogramms_Proteins_Gauss);
			// Mean
			hist_obj_protein = VectorXf::Zero(HIST_SIZE);
			op = getBasePath() + "resources/curvature/dataset/Mean/" + to_string(i) + ".txt";
			open_curves(op, H_protein);
			hist = getBasePath() + "resources/histogram/curvature_dataset/Mean/" + to_string(i) + ".txt";
			calc_histogramm(min_Mean, max_Mean, H_protein, hist_obj_protein, hist);
			H_protein.clear();
			add_histProtein(i, histogramms_Proteins_Mean);
		}
		// Υπολογισμός dissimilarity matrix
		cout << "Calculate dissmilarity Matrix" << endl;
		string op = "gauss_mean_" + to_string(static_cast<int>(WEIGHT_GAUSS*10)) + ".txt";
		string name = getBasePath() + "resources/curvature_dissimilarity_matrix/arrays/" + op;
		cout << "Calculate dissmilarity Matrix precent_gauss: " << WEIGHT_GAUSS << endl;
		calc_dissimilarity_matrix_combination(histogramms_Proteins_Gauss, histogramms_Proteins_Mean, dissimilarity_matrix_proteins, name, WEIGHT_GAUSS);
		cout << "Print dissimilarity matrix" << endl;
	}
	//---Task 5---//
	else if (TASK == 5) {
		double errors = 0;
		dissimilarity_matrix_proteins = MatrixXf::Zero(369, 369);
		string op = "gauss_mean_" + to_string(static_cast<int>(WEIGHT_GAUSS * 10)) + ".txt";
		cout << op << endl;
		string name_diss = getBasePath() + "resources/curvature_dissimilarity_matrix/arrays/" + op;
		open_dissimilarity_matrix(name_diss, dissimilarity_matrix_proteins);
		dissimilarity_matrix_proteins_bool = MatrixXf::Zero(369, 369);
		int teams[5] = { 44, 116, 161, 226, 368 };
		cout << "Bounder T = " << T << "->";
		string name_boolean = getBasePath() + "resources/curvature_dissimilarity_matrix/boolean/diss_matrix_bool.txt";
		similar_protein(T, name_boolean);
		string op2 = "pr_" + to_string(static_cast<int>(WEIGHT_GAUSS * 10)) + "_class.txt";
		string name_c = getBasePath() + "resources/curvature_dissimilarity_matrix/classification/" + op2;
		errors = classification_protein(teams, name_c);
	}
	//---Task 6---//
	else if (TASK == 6) {
		// Υπολογισμός ιστογραμματων καμπυλοτητων test_set
		if (ON_FILE == 1) {
			cout << "Calculate histogramm test set" << endl;
			max_Gauss = 216.969;
			min_Gauss = -0.493732;
			max_Mean = 4.07601;
			min_Mean = 0.0257181;
			for (int i = 0; i < 41; i++) {
				string name = to_string(i) + ".txt";
				cout << name << endl;
				// Gauss
				hist_obj_protein = VectorXf::Zero(HIST_SIZE);
				string op = getBasePath() + "resources/curvature/test_set/Gauss/" + to_string(i) + ".txt";
				open_curves(op, K_protein);
				string hist = getBasePath() + "resources/histogram/curvature_test_set/Gauss/" + to_string(i) + ".txt";
				calc_histogramm(min_Gauss, max_Gauss, K_protein, hist_obj_protein, hist);
				K_protein.clear();
				// Mean
				hist_obj_protein = VectorXf::Zero(HIST_SIZE);
				op = getBasePath() + "resources/curvature/test_set/Mean/" + to_string(i) + ".txt";
				open_curves(op, H_protein);
				hist = getBasePath() + "resources/histogram/curvature_test_set/Mean/" + to_string(i) + ".txt";
				calc_histogramm(min_Mean, max_Mean, H_protein, hist_obj_protein, hist);
				H_protein.clear();
			}
		}
		Vec_similar_protein = VectorXf::Zero(41);
		find_similar_vector();
		int teams[5] = { 4, 12, 17, 22, 40 };
		retriaval_degree(teams);
	}
	//---Task 7---//
	else if (TASK == 7) {
		max_col_0 = 35.35;
		min_col_0 = -30.1416;
		max_col_1 = 4.5;
		min_col_1 = -4.5;
		max_col_2 = 1;
		min_col_2 = -1;
		if (ON_FILE == 1) {
			// dataset
			for (int i = 0; i < 369; i++) {
				hist_properties_col_0 = VectorXf::Zero(HIST_SIZE);
				hist_properties_col_1 = VectorXf::Zero(HIST_SIZE);
				hist_properties_col_2 = VectorXf::Zero(HIST_SIZE);
				string name = to_string(i) + ".txt";
				cout << name << endl;
				string op = getBasePath() + "resources/dataset/properties/" + to_string(i) + ".txt";
				open_physicochemical_properties(op, protein_properties_col_0, protein_properties_col_1, protein_properties_col_2);
				string hist0_ = getBasePath() + "resources/histogram/properties_dataset/0/" + to_string(i) + ".txt";
				string hist1_ = getBasePath() + "resources/histogram/properties_dataset/1/" + to_string(i) + ".txt";
				string hist2_ = getBasePath() + "resources/histogram/properties_dataset/2/" + to_string(i) + ".txt";
				calc_histogramm(min_col_0, max_col_0, protein_properties_col_0, hist_properties_col_0, hist0_);
				calc_histogramm(min_col_1, max_col_1, protein_properties_col_1, hist_properties_col_1, hist1_);
				calc_histogramm(min_col_2, max_col_2, protein_properties_col_2, hist_properties_col_2, hist2_);
				protein_properties_col_0.clear();
				protein_properties_col_1.clear();
				protein_properties_col_2.clear();
			}
			// test_set
			for (int i = 0; i < 41; i++) {
				hist_properties_col_0 = VectorXf::Zero(HIST_SIZE);
				hist_properties_col_1 = VectorXf::Zero(HIST_SIZE);
				hist_properties_col_2 = VectorXf::Zero(HIST_SIZE);
				string name = to_string(i) + ".txt";
				cout << name << endl;
				string op = getBasePath() + "resources/test_set/properties/" + to_string(i) + ".txt";
				open_physicochemical_properties(op, protein_properties_col_0, protein_properties_col_1, protein_properties_col_2);
				string hist0_ = getBasePath() + "resources/histogram/properties_test_set/0/" + to_string(i) + ".txt";
				string hist1_ = getBasePath() + "resources/histogram/properties_test_set/1/" + to_string(i) + ".txt";
				string hist2_ = getBasePath() + "resources/histogram/properties_test_set/2/" + to_string(i) + ".txt";
				calc_histogramm(min_col_0, max_col_0, protein_properties_col_0, hist_properties_col_0, hist0_);
				calc_histogramm(min_col_1, max_col_1, protein_properties_col_1, hist_properties_col_1, hist1_);
				calc_histogramm(min_col_2, max_col_2, protein_properties_col_2, hist_properties_col_2, hist2_);
				protein_properties_col_0.clear();
				protein_properties_col_1.clear();
				protein_properties_col_2.clear();
			}
		}
		// --Task 4-- //
		arr_hist_properties_col_0 = MatrixXf::Zero(369, HIST_SIZE);
		arr_hist_properties_col_1 = MatrixXf::Zero(369, HIST_SIZE);
		arr_hist_properties_col_2 = MatrixXf::Zero(369, HIST_SIZE);
		for (int i = 0; i < 369; i++) {
			hist_properties_col_0 = VectorXf::Zero(HIST_SIZE);
			hist_properties_col_1 = VectorXf::Zero(HIST_SIZE);
			hist_properties_col_2 = VectorXf::Zero(HIST_SIZE);
			string name = to_string(i) + ".txt";
			cout << name << endl;
			string hist0 = getBasePath() + "resources/histogram/properties_dataset/0/" + to_string(i) + ".txt";
			string hist1 = getBasePath() + "resources/histogram/properties_dataset/1/" + to_string(i) + ".txt";
			string hist2 = getBasePath() + "resources/histogram/properties_dataset/2/" + to_string(i) + ".txt";
			open_hist(hist0, hist_properties_col_0);
			open_hist(hist1, hist_properties_col_1);
			open_hist(hist2, hist_properties_col_2);
			arr_hist_properties_col_0.row(i) = hist_properties_col_0;
			arr_hist_properties_col_1.row(i) = hist_properties_col_1;
			arr_hist_properties_col_2.row(i) = hist_properties_col_2;
		}
		// --Task 5-- //
		string name;
		// prop 0
		string diss0 = getBasePath() + "resources/properties_dissimilarity_matrix/arrays/0.txt";
		diss_properties_col_0 = MatrixXf::Zero(369, 369);
		calc_dissimilarity_matrix_prop(arr_hist_properties_col_0, diss_properties_col_0, diss0);
		diss_matrix_prop_bool = MatrixXf::Zero(369, 369);
		name = getBasePath() + "resources/properties_dissimilarity_matrix/boolean/0.txt";
		similar_prop(T, diss_properties_col_0, name);
		cout << "Prop 0->";
		c_prop_0 = VectorXf::Zero(369);
		classification_protein_prop(diss_properties_col_0, c_prop_0);
		// prop 1
		string diss1 = getBasePath() + "resources/properties_dissimilarity_matrix/arrays/1.txt";
		diss_properties_col_1 = MatrixXf::Zero(369, 369);
		calc_dissimilarity_matrix_prop(arr_hist_properties_col_1, diss_properties_col_1, diss1);
		diss_matrix_prop_bool = MatrixXf::Zero(369, 369);
		name = getBasePath() + "resources/properties_dissimilarity_matrix/boolean/1.txt";
		similar_prop(T, diss_properties_col_1, name);
		cout << "Prop 1->";
		c_prop_1 = VectorXf::Zero(369);
		classification_protein_prop(diss_properties_col_1, c_prop_1);
		// prop 2
		string diss2 = getBasePath() + "resources/properties_dissimilarity_matrix/arrays/2.txt";
		diss_properties_col_2 = MatrixXf::Zero(369, 369);
		calc_dissimilarity_matrix_prop(arr_hist_properties_col_2, diss_properties_col_2, diss2);
		diss_matrix_prop_bool = MatrixXf::Zero(369, 369);
		name = getBasePath() + "resources/properties_dissimilarity_matrix/boolean/2.txt";
		similar_prop(T, diss_properties_col_2, name);
		cout << "Prop 2->";
		c_prop_2 = VectorXf::Zero(369);
		classification_protein_prop(diss_properties_col_2, c_prop_2);
		// total
		string disst = getBasePath() + "resources/properties_dissimilarity_matrix/arrays/total.txt";
		diss_properties_total = MatrixXf::Zero(369, 369);
		calc_dissimilarity_matrix_prop_total(disst);
		diss_matrix_prop_bool = MatrixXf::Zero(369, 369);
		name = getBasePath() + "resources/properties_dissimilarity_matrix/boolean/total.txt";
		similar_prop(T, diss_properties_total, name);
		cout << "Prop all->";
		c_prop_total = VectorXf::Zero(369);
		classification_protein_prop(diss_properties_total, c_prop_total);
		// --Task 6-- //
		// prop 0
		Vec_similar_protein_col_0 = VectorXf::Zero(41);
		find_similar_vector_prop(Vec_similar_protein_col_0, "0/");
		cout << "Vec prop 0" << endl;
		retriaval_degree_prop(Vec_similar_protein_col_0, c_prop_0);
		// prop 1
		Vec_similar_protein_col_1 = VectorXf::Zero(41);
		find_similar_vector_prop(Vec_similar_protein_col_1, "1/");
		cout << "Vec prop 1" << endl;
		retriaval_degree_prop(Vec_similar_protein_col_1, c_prop_1);
		// prop 2
		Vec_similar_protein_col_2 = VectorXf::Zero(41);
		find_similar_vector_prop(Vec_similar_protein_col_2, "2/");
		cout << "Vec prop 2" << endl;
		retriaval_degree_prop(Vec_similar_protein_col_2, c_prop_2);
		// total
		Vec_similar_protein_col_total = VectorXf::Zero(41);
		find_similar_vector_prop_total();
		cout << "Vec prop total" << endl;
		retriaval_degree_prop(Vec_similar_protein_col_total, c_prop_total);
	}
}

// Task 0
/* Υπολογίζει και αποθηκεύει τις καμπυλότητες σε αρχεία .txt του συνόλου εκπαίδευσης*/
void Mesh3DScene::load_dataset_curves() {
	for (int i = 0; i < 369; i++) {
		// Φόρτωση αντίστοιχου αντικειμένου
		string name = to_string(i) + ".obj";
		cout << name << endl;
		const string objFile = objDirDataset + name;
		m_model_protein = vvr::Mesh(objFile);
		// Υπολογισμός Gauss και Mean
		find_vertex_neighbours(m_model_protein, m_NeighList_protein);
		find_Normal_vertices_surface(m_model_protein, m_NeighList_protein);
		calc_gauss_curve(m_model_protein, m_NeighList_protein, K_protein);
		vector<double> vd_K1 = find_max_min_value(K_protein);
		double min_K = vd_K1[0];
		double max_K = vd_K1[1];
		if (i == 0) {
			max_Gauss = max_K;
			min_Gauss = min_K;
		}
		if (max_Gauss < max_K) {
			max_Gauss = max_K;
		}
		if (min_Gauss > min_K) {
			min_Gauss = min_K;
		}
		string op_K = getBasePath() + "resources/curvature/dataset/Gauss/" + to_string(i) + ".txt";
		ofstream myfile1(op_K);
		if (myfile1.is_open()) {
			for (int j = 0; j < K_protein.size(); j++) {
				double lambda = K_protein[j];
				myfile1 << lambda << " ";
			}
			myfile1.close();
		}
		else cout << "Unable to open file";
		calc_mean_curve(m_model_protein, m_NeighList_protein, H_protein);
		vector<double> vd_H1 = find_max_min_value(H_protein);
		double min_H = vd_H1[0];
		double max_H = vd_H1[1];
		if (i == 0) {
			max_Mean = max_H;
			min_Mean = min_H;
		}
		if (max_Gauss < max_K) {
			max_Mean = max_H;
		}
		if (min_Gauss > min_K) {
			min_Mean = min_H;
		}
		string op_H = getBasePath() + "resources/curvature/dataset/Mean/" + to_string(i) + ".txt";
		ofstream myfile2(op_H);
		if (myfile2.is_open()) {
			for (int j = 0; j < H_protein.size(); j++) {
				double lambda = H_protein[j];
				myfile2 << lambda << " ";
			}
			myfile2.close();
		}
		else cout << "Unable to open file";
		m_NeighList_protein.clear();
		K_protein.clear();
		H_protein.clear();
	}
	// Αποθήκευση μέγιστης και ελάχιστης τιμής
	string op_Km = getBasePath() + "resources/curvature/dataset/Gauss/min_max.txt";
	string op_Hm = getBasePath() + "resources/curvature/dataset/Mean/min_max.txt";
	ofstream myfile1(op_Km);
	if (myfile1.is_open()) {
		myfile1 << min_Gauss << " " << max_Gauss;
		myfile1.close();
	}
	else cout << "Unable to open file";
	ofstream myfile2(op_Hm);
	if (myfile2.is_open()) {
		myfile2 << min_Mean << " " << max_Mean;
		myfile2.close();
	}
	else cout << "Unable to open file";
}
/* Υπολογίζει και αποθηκεύει τις καμπυλότητες σε αρχεία .txt του συνόλου δοκιμής*/
void Mesh3DScene::load_test_set_curves() {
	for (int i = 0; i < 41; i++) {
		// Φόρτωση αντίστοιχου αντικειμένου
		string name = to_string(i) + ".obj";
		cout << name << endl;
		const string objFile = objDirTestset + name;
		m_model_protein = vvr::Mesh(objFile);
		// Υπολογισμός Gauss και Mean
		find_vertex_neighbours(m_model_protein, m_NeighList_protein);
		find_Normal_vertices_surface(m_model_protein, m_NeighList_protein);
		calc_gauss_curve(m_model_protein, m_NeighList_protein, K_protein);
		vector<double> vd_K1 = find_max_min_value(K_protein);
		double min_K = vd_K1[0];
		double max_K = vd_K1[1];
		if (i == 0) {
			max_Gauss = max_K;
			min_Gauss = min_K;
		}
		if (max_Gauss < max_K) {
			max_Gauss = max_K;
		}
		if (min_Gauss > min_K) {
			min_Gauss = min_K;
		}
		string op_K = getBasePath() + "resources/curvature/test_set/Gauss/" + to_string(i) + ".txt";
		ofstream myfile1(op_K);
		if (myfile1.is_open()) {
			for (int j = 0; j < K_protein.size(); j++) {
				double lambda = K_protein[j];
				myfile1 << lambda << " ";
			}
			myfile1.close();
		}
		else cout << "Unable to open file";
		calc_mean_curve(m_model_protein, m_NeighList_protein, H_protein);
		vector<double> vd_H1 = find_max_min_value(H_protein);
		double min_H = vd_H1[0];
		double max_H = vd_H1[1];
		if (i == 0) {
			max_Mean = max_H;
			min_Mean = min_H;
		}
		if (max_Gauss < max_K) {
			max_Mean = max_H;
		}
		if (min_Gauss > min_K) {
			min_Mean = min_H;
		}
		string op_H = getBasePath() + "resources/curvature/test_set/Mean/" + to_string(i) + ".txt";
		ofstream myfile2(op_H);
		if (myfile2.is_open()) {
			for (int j = 0; j < H_protein.size(); j++) {
				double lambda = H_protein[j];
				myfile2 << lambda << " ";
			}
			myfile2.close();
		}
		else cout << "Unable to open file";
		m_NeighList_protein.clear();
		K_protein.clear();
		H_protein.clear();
	}
	// Αποθήκευση μέγιστης και ελάχιστης τιμής
	string op_Km = getBasePath() + "resources/curvature/test_set/Gauss/min_max.txt";
	string op_Hm = getBasePath() + "resources/curvature/test_set/Mean/min_max.txt";
	ofstream myfile1(op_Km);
	if (myfile1.is_open()) {
		myfile1 << min_Gauss << " " << max_Gauss;
		myfile1.close();
	}
	else cout << "Unable to open file";
	ofstream myfile2(op_Hm);
	if (myfile2.is_open()) {
		myfile2 << min_Mean << " " << max_Mean;
		myfile2.close();
	}
	else cout << "Unable to open file";
}
/* Ξεχωρίζει και αποθηκεύει τις φυσικοχημικές ιδιότητες σε αρχεία .txt του συνόλου εκπαίδευσης*/
void Mesh3DScene::load_physhiomedical_properties_dataset() {
	for (int i = 0; i < 369; i++) {
		// Φόρτωση φυσιοχημικών ιδιοτήτων ενός αντικειμένου
		string name = to_string(i) + ".txt";
		cout << name << endl;
		const string objFile = objDirPropDataset + name;
		// ’νοιγμα αρχείων ιδιοτήτων
		open_physicochemical_properties(objFile, protein_properties_col_0, protein_properties_col_1, protein_properties_col_2);
		vector<double> vd_0 = find_max_min_value(protein_properties_col_0);
		vector<double> vd_1 = find_max_min_value(protein_properties_col_1);
		vector<double> vd_2 = find_max_min_value(protein_properties_col_2);
		double min_c0 = vd_0[0];
		double max_c0 = vd_0[1];
		double min_c1 = vd_1[0];
		double max_c1 = vd_1[1];
		double min_c2 = vd_2[0];
		double max_c2 = vd_2[1];
		if (i == 0) {
			max_col_0 = max_c0;
			min_col_0 = min_c0;
			max_col_1 = max_c1;
			min_col_1 = min_c1;
			max_col_2 = max_c2;
			min_col_2 = min_c2;
		}
		if (max_Gauss < max_c0) {
			max_col_0 = max_c0;
		}
		if (min_Gauss > min_c0) {
			min_col_0 = min_c0;
		}
		if (max_Gauss < max_c1) {
			max_col_1 = max_c1;
		}
		if (min_Gauss > min_c1) {
			min_col_1 = min_c1;
		}
		if (max_Gauss < max_c2) {
			max_col_2 = max_c2;
		}
		if (min_Gauss > min_c2) {
			min_col_2 = min_c2;
		}
	}
	// Αποθήκευση μέγιστης και ελάχιστης τιμής
	string op0 = getBasePath() + "resources/properties/dataset/prop_0_min_max.txt";
	string op1 = getBasePath() + "resources/properties/dataset/prop_1_min_max.txt";
	string op2 = getBasePath() + "resources/properties/dataset/prop_2_min_max.txt";
	ofstream myfile1(op0);
	if (myfile1.is_open()) {
		myfile1 << min_col_0 << " " << max_col_0;
		myfile1.close();
	}
	else cout << "Unable to open file";
	ofstream myfile2(op1);
	if (myfile2.is_open()) {
		myfile2 << min_col_1 << " " << max_col_1;
		myfile2.close();
	}
	else cout << "Unable to open file";
	ofstream myfile3(op2);
	if (myfile3.is_open()) {
		myfile3 << min_col_2 << " " << max_col_2;
		myfile3.close();
	}
	else cout << "Unable to open file";
	protein_properties_col_0.clear();
	protein_properties_col_1.clear();
	protein_properties_col_2.clear();

}
/* Ξεχωρίζει και αποθηκεύει τις φυσικοχημικές ιδιότητες σε αρχεία .txt του συνόλου δοκιμής*/
void Mesh3DScene::load_physhiomedical_properties_test_set() {
	for (int i = 0; i < 41; i++) {
		// Φόρτωση φυσιοχημικών ιδιοτήτων ενός αντικειμένου
		string name = to_string(i) + ".txt";
		cout << name << endl;
		const string objFile = objDirPropTestset + name;
		// ’νοιγμα αρχείων ιδιοτήτων
		open_physicochemical_properties(objFile, protein_properties_col_0, protein_properties_col_1, protein_properties_col_2);
		vector<double> vd_0 = find_max_min_value(protein_properties_col_0);
		vector<double> vd_1 = find_max_min_value(protein_properties_col_1);
		vector<double> vd_2 = find_max_min_value(protein_properties_col_2);
		double min_c0 = vd_0[0];
		double max_c0 = vd_0[1];
		double min_c1 = vd_1[0];
		double max_c1 = vd_1[1];
		double min_c2 = vd_2[0];
		double max_c2 = vd_2[1];
		if (i == 0) {
			max_col_0 = max_c0;
			min_col_0 = min_c0;
			max_col_1 = max_c1;
			min_col_1 = min_c1;
			max_col_2 = max_c2;
			min_col_2 = min_c2;
		}
		if (max_Gauss < max_c0) {
			max_col_0 = max_c0;
		}
		if (min_Gauss > min_c0) {
			min_col_0 = min_c0;
		}
		if (max_Gauss < max_c1) {
			max_col_1 = max_c1;
		}
		if (min_Gauss > min_c1) {
			min_col_1 = min_c1;
		}
		if (max_Gauss < max_c2) {
			max_col_2 = max_c2;
		}
		if (min_Gauss > min_c2) {
			min_col_2 = min_c2;
		}
	}
	// Αποθήκευση μέγιστης και ελάχιστης τιμής
	string op0 = getBasePath() + "resources/properties/test_set/prop_0_min_max_test.txt";
	string op1 = getBasePath() + "resources/properties/test_set/prop_1_min_max_test.txt";
	string op2 = getBasePath() + "resources/properties/test_set/prop_2_min_max_test.txt";
	ofstream myfile1(op0);
	if (myfile1.is_open()) {
		myfile1 << min_col_0 << " " << max_col_0;
		myfile1.close();
	}
	else cout << "Unable to open file";
	ofstream myfile2(op1);
	if (myfile2.is_open()) {
		myfile2 << min_col_1 << " " << max_col_1;
		myfile2.close();
	}
	else cout << "Unable to open file";
	ofstream myfile3(op2);
	if (myfile3.is_open()) {
		myfile3 << min_col_2 << " " << max_col_2;
		myfile3.close();
	}
	else cout << "Unable to open file";
	protein_properties_col_0.clear();
	protein_properties_col_1.clear();
	protein_properties_col_2.clear();
}
/* Εύρεση μέγιστης και ελάχιστης τιμής σε ένα διάνυσμα*/
vector<double> Mesh3DScene::find_max_min_value(vector<double> VecList) {
	double max_v = 0;
	double min_v = 100;
	for (int i = 0; i < VecList.size(); i++) {
		if (max_v < VecList[i]) {
			max_v = VecList[i];
		}
		if (min_v > VecList[i]) {
			min_v = VecList[i];
		}
	}
	vector<double> min_max = { min_v, max_v };
	return min_max;
}
/* Εκπτύπωση διανύσματος VectorXf*/
void Mesh3DScene::print_VectorXf(Eigen::VectorXf& v) {
	for (int i = 0; i < v.rows(); i++) {
		cout << setw(4);
		cout << v[i];
	}
	cout << endl;
}
/* Εκπτύπωση διανύσματος MatrixXf*/
void Mesh3DScene::print_MatrixXf(Eigen::MatrixXf& arr) {
	for (int i = 0; i < arr.rows(); i++) {
		for (int j = 0; j < arr.cols(); j++) {
			cout << setw(11);
			cout << arr(i, j);
		}
		cout << endl;
	}
	cout << endl;
}
/* Υπολογισμός του σφάλματος δύο διανυσμάτων*/
float Mesh3DScene::calc_error(Eigen::VectorXf& v1, Eigen::VectorXf& v2) {
	VectorXf s = VectorXf::Zero(v1.rows());
	for (int i = 0; i < v1.rows(); i++) {
		s[i] = pow(v1[i] - v2[i], 2);
	}
	float sum_s = s.sum();
	return sum_s;
}
/* Εύρεση της θέσης της μέγιστης τιμής*/
int Mesh3DScene::max_index(Eigen::VectorXf v) {
	int m = 0;
	int ind = 0;
	for (int i = 0; i < v.rows(); i++) {
		if (m <= v(i)) {
			m = v(i);
			ind = i;
		}
	}
	return ind;
}
/* Εύρεση της θέσης της μικρότερης τιμής*/
int Mesh3DScene::min_index(Eigen::VectorXf v) {
	float m = 1.1;
	int ind = 0;
	for (int i = 0; i < v.rows(); i++) {
		if (m >= v(i)) {
			m = v(i);
			ind = i;
		}
	}
	return ind;
}
/* Ορίζουμε τα διανύσματα ομάδων με τις σωστές ταξινομήσεις πρωτεΐνων*/
Eigen::VectorXf Mesh3DScene::build_c_correct(int teams[5], int size_c) {
	VectorXf c_correct = VectorXf::Zero(size_c);
	for (int i = 0; i < size_c; i++) {
		for (int k = 0; k < 5; k++) {
			if (i <= teams[k]) {
				c_correct(i) = k;
				break;
			}
		}
	}
	return c_correct;
}
/* Κανονικοποίηση διανύσματος VectorXf*/
void Mesh3DScene::normalize_VectorXf(Eigen::VectorXf& v) {
	float max_val = v.maxCoeff();
	for (int i = 0; i < v.rows(); i++) {
		v[i] /= max_val;
	}
}


//---Task 1---//
/* Αποθήκευση σε διανύσματα τις φυσικοχημικές ιδιότητες των πρωτεΐνων*/
void Mesh3DScene::open_physicochemical_properties(string file, std::vector<double>& m_properties_col_0, std::vector<double>& m_properties_col_1, std::vector<double>& m_properties_col_2) {
	ifstream myReadFile;
	myReadFile.open(file);
	string output;
	if (myReadFile.is_open()) {
		int count = 0;
		while (!myReadFile.eof()) {
			count++;
			myReadFile >> output;
			if (count == 1) {
				m_properties_col_0.push_back(stod(output));
			}
			else if (count == 2){
				m_properties_col_1.push_back(stod(output));
			}
			else if (count == 3) {
				count = 0;
				m_properties_col_2.push_back(stod(output));
			}
		}
	}
	myReadFile.close();
}
/* Εύρεση μέσης τιμής και τοποθέτησει στο αντίστοιχο τρίγωνο*/
void Mesh3DScene::find_median_value_triangle(std::vector<double>& properties_col, std::vector<double>& median_value_vector) {
	vector<vec> vertices = m_model.getVertices();
	vector<vvr::Triangle> triangles = m_model.getTriangles();
	for (int i = 0; i < triangles.size(); i++) {
		int v1 = triangles[i].vi1;
		int v2 = triangles[i].vi2;
		int v3 = triangles[i].vi3;
		double median1 = properties_col[v1];
		double median2 = properties_col[v2];
		double median3 = properties_col[v3];
		if ((median1 >= median2 && median1 <= median3) || (median1 <= median2 && median1 >= median3)) {
			median_value_vector.push_back(median1);
			
		}
		else if ((median2 >= median1 && median2 <= median3) || (median2 <= median1 && median2 >= median3)) {
			median_value_vector.push_back(median2);

		}
		else if ((median3 >= median2 && median3 <= median1) || (median3 <= median2 && median3 >= median1)) {
			median_value_vector.push_back(median3);
		}
	}
}
/* Συναρτήσεις τοποθέτησης χρώματος για τις τρεις ιδιότητες*/
void Mesh3DScene::find_colour_face_0(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face) {
	for (int i = 0; i < median_value_vector.size(); i++) {
		if (median_value_vector[i] >= 0) {
			int colour_intensity = floor(median_value_vector[i] * 200 / max_value_col_0);
			Colour c(255, 200 - colour_intensity, 200 - colour_intensity);
			colour_face.push_back(c);
		}
		if (median_value_vector[i] < 0) {
			int colour_intensity = floor(median_value_vector[i] * 200 / min_value_col_0);
			Colour c(200 - colour_intensity, 200 - colour_intensity, 255);
			colour_face.push_back(c);
		}
	}
}
void Mesh3DScene::find_colour_face_1(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face) {
	for (int i = 0; i < median_value_vector.size(); i++) {
		if (median_value_vector[i] >= 0) {
			int colour_intensity = floor(median_value_vector[i] * 200 / max_value_col_1);
			Colour c(255, 200 - colour_intensity, 255);
			colour_face.push_back(c);
		}
		if (median_value_vector[i] < 0) {
			int colour_intensity = floor(median_value_vector[i] * 200 / min_value_col_1);
			Colour c(200 - colour_intensity, 255, 255);
			colour_face.push_back(c);
		}
	}
}
void Mesh3DScene::find_colour_face_2(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face) {
	for (int i = 0; i < median_value_vector.size(); i++) {
		if (median_value_vector[i] >= 0) {
			int colour_intensity = floor(median_value_vector[i] * 200 / max_value_col_2);
			Colour c(200 - colour_intensity, 255, 200 - colour_intensity);
			colour_face.push_back(c);
		}
		if (median_value_vector[i] < 0) {
			int colour_intensity = floor(median_value_vector[i] * 200 / min_value_col_2);
			Colour c(255, 255, 200 - colour_intensity);
			colour_face.push_back(c);
		}
	}
}

//---Task 2---//
/* Συνάρτηση υπολογισμού καμπυλοτήτων*/
void Mesh3DScene::calc_gauss_mean_curve() {
	find_vertex_neighbours(m_model, m_NeighboursList);
	find_Normal_vertices_surface(m_model, m_NeighboursList);
	// Calculate gauss curve
	calc_gauss_curve(m_model, m_NeighboursList, K_gauss_curve);
	vector<double> vd_K = find_max_min_value(K_gauss_curve);
	min_value_K = vd_K[0];
	max_value_K = vd_K[1];
	find_median_value_triangle(K_gauss_curve, median_value_K);
	find_colour_face_gauss(median_value_K, colour_K);
	// Calculate mean curve
	calc_mean_curve(m_model, m_NeighboursList, H_mean_curve);
	vector<double> vd_H = find_max_min_value(H_mean_curve);
	min_value_H = vd_H[0];
	max_value_H = vd_H[1];
	find_median_value_triangle(H_mean_curve, median_value_H);
	find_colour_face_mean(median_value_H, colour_H);
}
/* Κατασκευή ενός διανύσματος της δομής vertex_neighbours*/
void Mesh3DScene::find_vertex_neighbours(vvr::Mesh m_model, std::vector<vertex_neighbours>& m_NeighboursList) {
	for (int i = 0; i < m_model.getVertices().size(); i++) {
		vertex_neighbours vn1;
		m_NeighboursList.push_back(vn1);
	}
	vector<vvr::Triangle>& triangles = m_model.getTriangles();
	for (int i = 0; i < triangles.size(); i++) {
		int v1 = triangles[i].vi1;
		int v2 = triangles[i].vi2;
		int v3 = triangles[i].vi3;
		// Πρόσθεση τριγώνου στο m_NeighboursList[v_]
		m_NeighboursList[v1].triangles.push_back(i);
		m_NeighboursList[v2].triangles.push_back(i);
		m_NeighboursList[v3].triangles.push_back(i);
		// Πρόσθεση γειτονικών σημείων στο m_NeighboursList[v_]
		// v1
		m_NeighboursList[v1].vertex.push_back(v2);
		m_NeighboursList[v1].vertex.push_back(v3);
		// v2
		m_NeighboursList[v2].vertex.push_back(v1);
		m_NeighboursList[v2].vertex.push_back(v3);
		// v3
		m_NeighboursList[v3].vertex.push_back(v1);
		m_NeighboursList[v3].vertex.push_back(v2);
	}
	// Τοποθέτηση στην σειρά τους κόμβους
	for (int i = 0; i < m_NeighboursList.size(); i++) {
		// Αλλαγή θέσεων
		for (int j = 1; j < m_NeighboursList[i].vertex.size(); j += 2) {
			for (int k = j + 1; k < m_NeighboursList[i].vertex.size(); k++) {
				if (m_NeighboursList[i].vertex[j] == m_NeighboursList[i].vertex[k]) {
					if (k == j + 1) {
						break;
					}
					else if (k == j + 2) {
						vector<int>::iterator it = m_NeighboursList[i].vertex.begin();
						int save_val = m_NeighboursList[i].vertex[k];
						m_NeighboursList[i].vertex.erase(it + k);
						m_NeighboursList[i].vertex.insert(it + j + 1, save_val);
					}
					else if (k % 2 == 0) {
						vector<int>::iterator it = m_NeighboursList[i].vertex.begin();
						int save_val = m_NeighboursList[i].vertex[k];
						m_NeighboursList[i].vertex.erase(it + k);
						m_NeighboursList[i].vertex.insert(it + j + 1, save_val);
						save_val = m_NeighboursList[i].vertex[k + 1];
						m_NeighboursList[i].vertex.erase(it + k + 1);
						m_NeighboursList[i].vertex.insert(it + j + 2, save_val);
						break;
					}
					else if (k % 2 == 1) {
						vector<int>::iterator it = m_NeighboursList[i].vertex.begin();
						int save_val = m_NeighboursList[i].vertex[k - 1];
						m_NeighboursList[i].vertex.erase(it + k - 1);
						m_NeighboursList[i].vertex.insert(it + j + 1, save_val);
						save_val = m_NeighboursList[i].vertex[k];
						m_NeighboursList[i].vertex.erase(it + k);
						m_NeighboursList[i].vertex.insert(it + j + 1, save_val);
						break;
					}
				}
			}
		}
		for (int l = m_NeighboursList[i].vertex.size() - 1; l > 0; l -= 2) {
			vector<int>::iterator it = m_NeighboursList[i].vertex.begin();
			m_NeighboursList[i].vertex.erase(it + l);
		}
	}
}
/* Υπολογισμός καμπυλότητας gauss*/
void Mesh3DScene::calc_gauss_curve(vvr::Mesh m_model, std::vector<vertex_neighbours>& m_NeighboursList, std::vector<double>& K_gauss_curve) {
	vector<vec>& vertices = m_model.getVertices();
	vector<vvr::Triangle>& triangles = m_model.getTriangles();
	for (int i = 0; i < vertices.size(); i++) {
		vertex_neighbours vn = m_NeighboursList[i];
		double angle = 0;
		double area_triangle = 0;
		int N = vn.vertex.size();
		for (int j = 0; j < N; j++) {
			angle += vertices[vn.vertex[j]].AngleBetween(vertices[vn.vertex[(j + 1) % N]]);
			math::Triangle tr(triangles[vn.vertex[j]].v1(), triangles[vn.vertex[j]].v2(), triangles[vn.vertex[j]].v3());
			area_triangle += tr.Area();
		}
		double K = 3 * (2 * pi - angle) / area_triangle;
		K_gauss_curve.push_back(K);
	}
}
/* Υπολογισμός του normal διανυσμάτων σε μία επιφάνεια*/
vec Mesh3DScene::calc_normal_surface(math::vec u, math::vec ui, math::vec ui_n) {
	vec num = (ui - u).Cross(ui_n - u);
	vec N = num.Normalized();
	return N;
}
/* Εύρεση των normal διανυσμάτων*/
void Mesh3DScene::find_Normal_vertices_surface(vvr::Mesh m_model, std::vector<vertex_neighbours>& m_NeighboursList) {
	vector<vec>& vertices = m_model.getVertices();
	for (int i = 0; i < vertices.size(); i++) {
		vertex_neighbours vn = m_NeighboursList[i];
		int L = vn.vertex.size();
		for (int j = 0; j < L; j++) {
			vec N = calc_normal_surface(vertices[i], vertices[vn.vertex[j]], vertices[vn.vertex[(j + 1) % L]]);
			m_NeighboursList[i].N_surface.push_back(N);
		}
	}
}
/* Υπολογισμός καμπυλότητας mean*/
void Mesh3DScene::calc_mean_curve(vvr::Mesh m_model, std::vector<vertex_neighbours>& m_NeighboursList, std::vector<double>& H_mean_curve) {
	vector<vec>& vertices = m_model.getVertices();
	vector<vvr::Triangle>& triangles = m_model.getTriangles();
	for (int i = 0; i < vertices.size(); i++) {
		vertex_neighbours vn = m_NeighboursList[i];
		double num = 0;
		double area_triangle = 0;
		int N = vn.vertex.size();
		for (int j = 0; j < N; j++) {
			double e = (vertices[vn.vertex[j]] - vertices[i]).Length();
			vec N_s = calc_normal_surface(vertices[i], vertices[vn.vertex[(j + 1) % N]], vertices[vn.vertex[(j + 2) % N]]);
			double b = vn.N_surface[j].AngleBetween(N_s);
			num += e * b;
			math::Triangle tr(triangles[vn.vertex[j]].v1(), triangles[vn.vertex[j]].v2(), triangles[vn.vertex[j]].v3());
			area_triangle += tr.Area();
		}
		double H = 0.75 * (num) / area_triangle;
		H_mean_curve.push_back(H);
	}
}
/* Συναρτήσεις τοποθέτησης χρώματος για τις καμπυλότητες*/
void Mesh3DScene::find_colour_face_gauss(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face) {
	for (int i = 0; i < median_value_vector.size(); i++) {
		if (median_value_vector[i] >= 0) {
			int colour_intensity = floor(median_value_vector[i] * 255 / max_value_K);
			Colour c(colour_intensity, 0, 0);
			colour_face.push_back(c);
		}
		if (median_value_vector[i] < 0) {
			int colour_intensity = floor(median_value_vector[i] * 255 / min_value_K);
			Colour c(0, 0, colour_intensity);
			colour_face.push_back(c);
		}
	}
}
void Mesh3DScene::find_colour_face_mean(std::vector<double>& median_value_vector, std::vector<vvr::Colour>& colour_face) {
	for (int i = 0; i < median_value_vector.size(); i++) {
		if (median_value_vector[i] >= 0) {
			int colour_intensity = floor(median_value_vector[i] * 255 / max_value_H);
			Colour c(255, colour_intensity, 0);
			colour_face.push_back(c);
		}
	}
}

//---Task 3---//
/* Εύρεση των διανυσμάτων του δυαδικού γράφου*/
void Mesh3DScene::find_dual_vertices() {
	cout << "find_dual_vertices()" << endl;
	int size_triangles = m_model.getTriangles().size();
	for (int i = 0; i < size_triangles; i++) {
		vvr::Triangle tri = m_model.getTriangles()[i];
		math::Triangle tri_math(tri.v1(), tri.v2(), tri.v3());
		vec center_mass = tri.getCenter();
		dual_vertice dv;
		dv.dual_v = center_mass;
		dv.index_vec = i;
		dv.count_neigh = -1;
		dv.area = tri_math.Area();
		Graph.push_back(dv);
	}
}
/* Εύρεση των γειτονικών τριγώνων*/
void Mesh3DScene::find_neigh_triangles() {
	cout << "find_neigh_triangles()" << endl;
	vector<vvr::Triangle>& triangles = m_model.getTriangles();
	int size_dual_list = Graph.size();
	int size_Neigh_Lsit = m_NeighboursList.size();
	for (int i = 0; i < size_Neigh_Lsit; i++) {
		for (int j = 0; j < m_NeighboursList[i].triangles.size(); j++) {
			int ind_j = m_NeighboursList[i].triangles[j];
			if (Graph[ind_j].count_neigh >= 2) {
				//cout << i << endl;
				continue;
			}
			for (int k = j + 1; k < m_NeighboursList[i].triangles.size(); k++) {
				int ind_k = m_NeighboursList[i].triangles[k];
				if (is_neighboors_triangles(triangles[ind_j], triangles[ind_k]) && 
					find(begin(Graph[ind_k].neigh_dual_vec), end(Graph[ind_k].neigh_dual_vec), ind_j) == end(Graph[ind_k].neigh_dual_vec)) {
					// +1 count
					Graph[ind_j].count_neigh++;
					Graph[ind_k].count_neigh++;
					// Γειτονικα διανύσματα
					Graph[ind_j].neigh_dual_vec[Graph[ind_j].count_neigh] = ind_k;
					Graph[ind_k].neigh_dual_vec[Graph[ind_k].count_neigh] = ind_j;
					// Απόσταση
					double dist = Graph[ind_j].dual_v.Distance(Graph[ind_k].dual_v);
					Graph[ind_j].distances[Graph[ind_j].count_neigh] = dist;
					Graph[ind_k].distances[Graph[ind_k].count_neigh] = dist;
				}
			}
		}
	}
}
/* Έλεγχος αν είναι γειτονικά*/
bool Mesh3DScene::is_neighboors_triangles(vvr::Triangle& tri1, vvr::Triangle& tri2) {
	// Πρώτο τρίγωνο
	vector<int> tri1_vec = { tri1.vi1, tri1.vi2, tri1.vi3 };
	// Δεύτερο τρίγωνο
	vector<int> tri2_vec = { tri2.vi1, tri2.vi2, tri2.vi3 };
	// Έλεγχος
	for (int i = 0; i < 3; i++) {
		int p1 = tri1_vec[i];
		int p2 = tri1_vec[(i+1)%3];
		for (int j = 0; j < 3; j++) {
			int q1 = tri2_vec[j];
			int q2 = tri2_vec[(j + 1) % 3];
			if ((p1 == q1 && p2 == q2) || (p1 == q2 && p2 == q1)) {
				return true;
			}
		}
	}
	return false;
}
/* Υπολογισμός του βαθμού προεξοχής*/
void Mesh3DScene::find_protrusion_degree() {
	max_p = 0;
	int size_heap = Graph.size();
	for (int i = 0; i < size_heap; i++) {
		for (int j = 0; j < size_heap; j++) {
			Graph[j].read = false;
			heap_node h;
			h.geodesic = std::numeric_limits<double>::infinity();
			h.index_vec = Graph[j].index_vec;
			heap_Dij.heap_D.push_back(h);
		}
		heap_Dij.HEAP_init(i);
		dijkstra(i);
		if (max_p < Graph[i].p) max_p = Graph[i].p;
	}
	cout << max_p << endl;
	for (int i = 0; i < size_heap; i++) {
		Graph[i].p /= max_p;
	}
}
/* Αλγόριθμος Dijkstra*/
void Mesh3DScene::dijkstra(int i) {
	while (heap_Dij.plithos > 0) {
		heap_node current = heap_Dij.HEAP_pop();
		dual_vertice& dv = Graph[current.index_vec];
		dv.geodesic = current.geodesic;
		dv.read = true;
		Graph[i].p += dv.geodesic * dv.area;
		for (int neigh = 0; neigh < dv.count_neigh + 1; neigh++) {
			if (Graph[dv.neigh_dual_vec[neigh]].read == false) {
				int index_neigh = dv.neigh_dual_vec[neigh];
				double distance = dv.geodesic + dv.distances[neigh];
				heap_Dij.change_data(index_neigh, distance);
			}
		}
	}
}
/*Αλγόριθμος N-rings*/
void Mesh3DScene::N_ring_algorithm() {
	for (int i = 0; i < m_model.getTriangles().size(); i++) {
		is_projection.push_back(0);
		Graph[i].read = false;
	}
	int count_vec = 0;
	int N_ring = floor(m_model.getTriangles().size() * 0.75);
	int i = 0;
	vector<dual_vertice> queue;
	queue.push_back(Graph[count_vec]);
	double max_projection_degree = 0;
	queue.front().read = true;
	while (count_vec < m_model.getTriangles().size()) {
		for (int j = 0; j < queue[i].count_neigh + 1; j++) {
			dual_vertice& dv = Graph[queue[i].neigh_dual_vec[j]];
			if (!dv.read) {
				queue.push_back(dv);
				dv.read = true;
			}
		}
		i++;
		if (i >= N_ring) {
			int index_max_projection = -1;
			for (int j = 0; j < queue.size(); j++) {
				if (queue[j].p > max_projection_degree) {
					max_projection_degree = queue[j].p;
					index_max_projection = j;
				}
			}
			if(max_projection_degree > 0.8){
				is_projection[queue[index_max_projection].index_vec] = 1;
			}
			count_vec++;
			for (int k = 0; k < m_model.getTriangles().size(); k++) Graph[k].read = false;
			max_projection_degree = 0;
			i = 0;
			queue.clear();
			if (count_vec == m_model.getTriangles().size()) break;
			queue.push_back(Graph[count_vec]);
			queue.front().read = true;
		}
	}
}

//--- Heap functions ---//
/* Αρχικοποίηση του σωρού*/
void Heap::HEAP_init(int i) {
	change_data(i, 0);
	plithos = heap_D.size();
}
/* Αφαίρεση ρίζας*/
heap_node Heap::HEAP_pop() {
	int posCurrent, posLeft, posRight, pos;
	heap_node pop_x = heap_D.front();
	swap_heap_node(heap_D[0], heap_D[plithos - 1]);
	plithos--;
	heap_D.pop_back();
	posCurrent = 0;
	while (posCurrent < plithos) {
		posLeft = 2 * posCurrent + 1;
		posRight = 2 * posCurrent + 2;

		if (posLeft >= plithos) posLeft = -1;
		if (posRight >= plithos) posRight = -1;

		// Δεν έχει παιδιά
		if (posLeft == -1 && posRight == -1) break;

		// Έχει μόνο αριστερό παιδί
		else if (posLeft != -1 && posRight == -1) {
			if (heap_D[posCurrent].geodesic > heap_D[posLeft].geodesic) {
				swap_heap_node(heap_D[posCurrent], heap_D[posLeft]);
				posCurrent = posLeft;
			}
			else break;
		}

		// Έχει δύο παιδιά
		else {
			if (heap_D[posLeft].geodesic > heap_D[posRight].geodesic) pos = posRight;
			else pos = posLeft;

			if (heap_D[posCurrent].geodesic > heap_D[pos].geodesic) {
				swap_heap_node(heap_D[posCurrent], heap_D[pos]);
				posCurrent = pos;
			}
			else {
				break;
			}
		}
	}
	return pop_x;
}
/* Ανακατάταξη κόμβων όταν αλλάζει τιμή ένας από αυτούς*/
void Heap::change_data(int i, double x) {
	int k;
	for (k = 0; k < heap_D.size(); k++) {
		if (heap_D[k].index_vec == i) {
			if (x < heap_D[k].geodesic) heap_D[k].geodesic = x;
			break;
		}
	}
	int posParent, posCurrent, posLeft, posRight, pos;
	posCurrent = k;
	while (posCurrent > 0)
	{
		posParent = (posCurrent - 1) / 2;
		if (heap_D[posCurrent].geodesic < heap_D[posParent].geodesic) {
			swap_heap_node(heap_D[posCurrent], heap_D[posParent]);
			posCurrent = posParent;
		}
		else break;
	}
	posCurrent = k;
	while (posCurrent < plithos) {
		posLeft = 2 * posCurrent + 1;
		posRight = 2 * posCurrent + 2;

		if (posLeft >= plithos) posLeft = -1;
		if (posRight >= plithos) posRight = -1;

		// Δεν έχει παιδιά
		if (posLeft == -1 && posRight == -1) break;

		// Έχει μόνο αριστερό παιδί
		else if (posLeft != -1 && posRight == -1) {
			if (heap_D[posCurrent].geodesic > heap_D[posLeft].geodesic) {
				swap_heap_node(heap_D[posCurrent], heap_D[posLeft]);
				//swap(heap_D[posCurrent].geodesic, heap_D[posLeft].geodesic);
				posCurrent = posLeft;
			}
			else break;
		}

		// Έχει δύο παιδιά
		else {
			if (heap_D[posLeft].geodesic > heap_D[posRight].geodesic) pos = posRight;
			else pos = posLeft;

			if (heap_D[posCurrent].geodesic > heap_D[pos].geodesic) {
				swap_heap_node(heap_D[posCurrent], heap_D[pos]);
				//swap(heap_D[posCurrent].geodesic, heap_D[pos].geodesic);
				posCurrent = pos;
			}
			else {
				break;
			}
		}
	}
}
/* Εκπτύπωση σωρού*/
void Heap::HEAP_print() {
	for (int i = 0; i < heap_D.size(); i++) {
		cout << heap_D[i].geodesic << ",";
	}
	cout << endl;
}
/* Αλλάγη τιμή των κόμβων*/
void Heap::swap_heap_node(heap_node& dv1, heap_node& dv2) {
	swap(dv1.index_vec, dv2.index_vec);
	swap(dv1.geodesic, dv2.geodesic);
}

//---Task 4---//
/* Προσθήκη του ιστουγράμματος του αντικειμένου σε έναν πίνακα MatrixXf*/
void Mesh3DScene::add_histObj_MatrixXf() {
	int rows_matrix = histogramm_Obj.rows();
	int cols_matrix = histogramm_Obj.cols();
	for (int i = 0; i < cols_matrix; i++) {
		histogramm_Obj(0, i) = hist_obj_1(i) / hist_obj_1.sum();
		histogramm_Obj(1, i) = hist_obj_2(i) / hist_obj_2.sum();
		histogramm_Obj(2, i) = hist_obj_3(i) / hist_obj_3.sum();
	}
}
/* Υπολογισμός του πίνακα ανομοιομορφίας*/
void Mesh3DScene::calc_dissimilarity_matrix(Eigen::MatrixXf& arr, Eigen::MatrixXf& diss_Matrix) {
	int rows_matrix = arr.rows();
	int cols_matrix = arr.cols();
	for (int i = 0; i < rows_matrix; i++) {
		for (int j = 0; j < rows_matrix; j++) {
			VectorXf v1 = arr.row(i);
			VectorXf v2 = arr.row(j);
			diss_Matrix(i, j) = calc_error(v1, v2);
		}
	}
	// Κανονικοποίηση πίνακα
	float max_val = diss_Matrix.maxCoeff();
	for (int i = 0; i < rows_matrix; i++) {
		for (int j = 0; j < rows_matrix; j++) {
			diss_Matrix(i, j) /= max_val;
		}
	}
}
/* Υπολογισμός του ιστογράμματος*/
void Mesh3DScene::calc_histogramm(double min_value, double max_value, std::vector<double>& data, Eigen::VectorXf& hist, std::string name) {
	for (int i = 0; i < data.size(); i++) {
		double norm = (data[i] - min_value) / (max_value - min_value);
		int index_hist = 0;
		if (norm >= 1) {
			index_hist = HIST_SIZE - 1;
		}
		else if (norm <= 0) {
			index_hist = 0;
		}
		else {
			index_hist = floor(HIST_SIZE * norm);
		}
		hist[index_hist]++;
	}
	// Καταγραφή σε txt
	ofstream myfile(name);
	if (myfile.is_open()) {
		for (int i = 0; i < hist.size(); i++) {
			int lambda = hist[i];
			myfile << lambda << "\n";
		}
		myfile.close();
	}
	else cout << "Unable to open file";
}
/* ’νοιγμα του αρχείου που περιέχει την καμπυλότητα του αντικειμένου και προσθήκηκ σε ένα διάνυσμα*/
void Mesh3DScene::open_curves(std::string file, std::vector<double>& cuerve) {
	ifstream myReadFile;
	myReadFile.open(file);
	string output;
	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {
			myReadFile >> output;
			cuerve.push_back(stod(output));
		}
	}
	myReadFile.close();
}
/* Προσθήκη του ιστουγράμματος του αντικειμένου σε έναν πίνακα MatrixXf*/
void Mesh3DScene::add_histProtein(int index, Eigen::MatrixXf& histogramms_Proteins) {
	int rows_matrix = histogramms_Proteins.rows();
	int cols_matrix = histogramms_Proteins.cols();
	for (int i = 0; i < cols_matrix; i++) {
		histogramms_Proteins(index, i) = hist_obj_protein(i);
	}
}
/* Υπολογισμός του πίνακα ανομοιομορφίας των πρωτεϊνών*/
void Mesh3DScene::calc_dissimilarity_matrix_combination(Eigen::MatrixXf& gauss, Eigen::MatrixXf& mean, Eigen::MatrixXf& diss_Matrix, std::string name, float pr_gauss) {
	MatrixXf diss_gauss = MatrixXf::Zero(369, 369);
	MatrixXf diss_mean = MatrixXf::Zero(369, 369);
	int rows_matrix = diss_gauss.rows();
	int cols_matrix = diss_gauss.cols();
	// dissimilarity gauss
	for (int i = 0; i < rows_matrix; i++) {
		for (int j = 0; j < rows_matrix; j++) {
			VectorXf v1 = gauss.row(i);
			VectorXf v2 = gauss.row(j);
			diss_gauss(i, j) = calc_error(v1, v2);
		}
	}
	// dissimilarity mean
	for (int i = 0; i < rows_matrix; i++) {
		for (int j = 0; j < rows_matrix; j++) {
			VectorXf v1 = mean.row(i);
			VectorXf v2 = mean.row(j);
			diss_mean(i, j) = calc_error(v1, v2);
		}
	}
	// dissimilarity matrix
	diss_Matrix = pr_gauss * diss_gauss + (1 - pr_gauss) * diss_mean;
	float max_val = diss_Matrix.maxCoeff();
	for (int i = 0; i < rows_matrix; i++) {
		for (int j = 0; j < rows_matrix; j++) {
			diss_Matrix(i, j) /= max_val;
		}
	}
	ofstream myfile(name);
	if (myfile.is_open()) {
		for (int i = 0; i < 369; i++) {
			for (int j = 0; j < 369; j++) {
				float lambda = diss_Matrix(i, j);
				myfile << lambda << " ";
			}
			myfile << "\n";
		}
		myfile.close();
	}
	else cout << "Unable to open file";
}

//---Task 5---//
/* ’νοιγμα αρχείου που περιέχει τον πίνακα ανομοιομορφίας*/
void Mesh3DScene::open_dissimilarity_matrix(std::string file, Eigen::MatrixXf& diss_Matrix) {
	ifstream myReadFile;
	myReadFile.open(file);
	string output;
	int i = 0;
	int j = 0;
	if (myReadFile.is_open()) {
		while (!myReadFile.eof() && i < 369) {
			myReadFile >> output;
			diss_Matrix(i, j) = stod(output);
			if (j < 368) {
				j++;
			}
			else {
				j = 0;
				i++;
			}
		}
	}
	myReadFile.close();
}
/* Υπολογισμός του πίνακα ομοιομορφίας των πρωτεϊνών με συγκεκριμένο κατώφλι*/
void Mesh3DScene::similar_protein(float Thres, std::string name) {
	int rows_matrix = dissimilarity_matrix_proteins_bool.rows();
	int cols_matrix = dissimilarity_matrix_proteins_bool.cols();
	for (int i = 0; i < rows_matrix; i++) {
		for (int j = 0; j < rows_matrix; j++) {
			if (dissimilarity_matrix_proteins(i, j) <= Thres && i != j) {
				dissimilarity_matrix_proteins_bool(i, j) = 1;
			}
		}
	}
	ofstream myfile(name);
	if (myfile.is_open()) {
		for (int i = 0; i < 369; i++) {
			for (int j = 0; j < 369; j++) {
				float lambda = dissimilarity_matrix_proteins_bool(i, j);
				myfile << lambda << " ";
			}
			myfile << "\n";
		}
		myfile.close();
	}
	else cout << "Unable to open file";
}
/* Ταξινόμηση πρωτεϊνων*/
float Mesh3DScene::classification_protein(int teams[5], std::string name) {
	int size_c = 369;
	VectorXf c_correct = build_c_correct(teams, size_c);
	VectorXf c = VectorXf::Zero(369);
	int rows_matrix = dissimilarity_matrix_proteins_bool.rows();
	int cols_matrix = dissimilarity_matrix_proteins_bool.cols();
	for (int i = 0; i < rows_matrix; i++) {
		VectorXf Rep = VectorXf::Zero(5);
		for (int j = 0; j < rows_matrix; j++) {
			if (dissimilarity_matrix_proteins_bool(i, j) == 1) {
				for (int k = 0; k < 5; k++) {
					if (j <= teams[k]) {
						Rep[k]++;
						break;
					}
				}
			}
		}
		int class_prot = static_cast<int>(max_index(Rep));
		c(i) = class_prot;
	}
	// Αποθήκευση των ταξινομήσεων c
	ofstream myfile(name);
	if (myfile.is_open()) {
		for (int k = 0; k < c.rows(); k++) {
			float lambda = c(k);
			myfile << lambda << "\n";
		}
		myfile.close();
	}
	else cout << "Unable to open file";
	// Υπολογισμός σφαλματος
	VectorXf Rc = VectorXf::Zero(5);
	for (int i = 0; i < 369; i++) {
		if (c(i) == c_correct(i)) {
			Rc(static_cast<int>(c(i)))++;
		}
	}
	float error = 1 - Rc.sum() / 369;
	cout << "error classification: " << error << endl;
	cout << "classification: " << error << endl;
	print_VectorXf(c);
	return error;
}

//---Task 6---//
/* ’νοιγμα αρχείου που περιέχει το ιστόγραμμα του αντικειμένου*/
void Mesh3DScene::open_hist(std::string name, Eigen::VectorXf& hist) {
	ifstream myReadFile;
	myReadFile.open(name);
	string output;
	if (myReadFile.is_open()) {
		int i = 0;
		while (!myReadFile.eof() && i < HIST_SIZE) {
			myReadFile >> output;
			hist[i] = stod(output);
			i++;
		}
	}
	myReadFile.close();
}
/* ’νοιγμα αρχείου που περιέχει την ταξινόμηση των πρωτεϊνων σε ομάδες*/
void Mesh3DScene::open_class_weight_gauss(Eigen::VectorXf& c, std::string name) {
	ifstream myReadFile;
	myReadFile.open(name);
	string output;
	int i = 0;
	if (myReadFile.is_open()) {
		while (!myReadFile.eof() && i < 369) {
			myReadFile >> output;
			c[i] = stod(output);
			i++;
		}
	}
	else {
		cout << "Unable read the file";
	}
	myReadFile.close();
}
/* Εύρεση της πιο όμοιας πρωτεΐνης και προσθήκη σε διάνυσμα*/
void Mesh3DScene::find_similar_vector() {
	for (int i = 0; i < 41; i++) {
		similar_test_data = VectorXf::Zero(369);
		hist_obj_protein_testset_gauss = VectorXf::Zero(HIST_SIZE);
		hist_obj_protein_testset_mean = VectorXf::Zero(HIST_SIZE);
		string name_test = to_string(i) + ".txt";
		string hist_test_gauss = getBasePath() + "resources/histogram/curvature_test_set/Gauss/" + to_string(i) + ".txt";
		open_hist(hist_test_gauss, hist_obj_protein_testset_gauss);
		string hist_test_mean = getBasePath() + "resources/histogram/curvature_test_set/Mean/" + to_string(i) + ".txt";
		open_hist(hist_test_gauss, hist_obj_protein_testset_mean);
		for (int j = 0; j < 369; j++) {
			hist_obj_protein_dataset_gauss = VectorXf::Zero(HIST_SIZE);
			hist_obj_protein_dataset_mean = VectorXf::Zero(HIST_SIZE);
			string name_data = to_string(j) + ".txt";
			string hist_data_gauss = getBasePath() + "resources/histogram/curvature_dataset/Gauss/" + to_string(j) + ".txt";
			open_hist(hist_data_gauss, hist_obj_protein_dataset_gauss);
			string hist_data_mean = getBasePath() + "resources/histogram/curvature_dataset/Mean/" + to_string(j) + ".txt";
			open_hist(hist_data_mean, hist_obj_protein_dataset_mean);
			// Υπολογισμός Ομοιομορφίας
			float sim_gauss = calc_error(hist_obj_protein_testset_gauss, hist_obj_protein_dataset_gauss);
			float sim_mean = calc_error(hist_obj_protein_testset_mean, hist_obj_protein_dataset_mean);
			float t = (WEIGHT_GAUSS * sim_gauss + (1 - WEIGHT_GAUSS) * sim_mean);
			similar_test_data(j) = t;
		}
		normalize_VectorXf(similar_test_data);
		int index_min = min_index(similar_test_data);
		Vec_similar_protein(i) = index_min;
	}
}
/* Υπολογισμός ποσοστού ανάκτησης*/
void Mesh3DScene::retriaval_degree(int teams[5]) {
	int size_c = 41;
	VectorXf c_correct = build_c_correct(teams, size_c);
	cout << "Weight Gauss: " << WEIGHT_GAUSS << endl;
	VectorXf c = VectorXf::Zero(369);
	VectorXf c_test = VectorXf::Zero(41);
	string name = "pr_" + to_string(static_cast<int>(WEIGHT_GAUSS * 10)) + "_class.txt";
	string file_class_pr = getBasePath() + "resources/curvature_dissimilarity_matrix/classification/" + name;
	open_class_weight_gauss(c, file_class_pr);
	for (int j = 0; j < Vec_similar_protein.rows(); j++) {
		c_test[j] = c[Vec_similar_protein[j]];
	}
	cout << "T: " << T << "->";
	// Εκπτήπωση των ομάδων του συνόλου δοκιμής
	print_VectorXf(c_test);
	VectorXf Rc = VectorXf::Zero(5);
	for (int j = 0; j < Vec_similar_protein.rows(); j++) {
		if (c_test[j] == c_correct(j)) {
			Rc[c_test[j]] += 1;
		}
	}
	// retrieval
	float retrieval = Rc.sum() / 41;
	cout << "retrieval:" << retrieval << endl;
}

//---Task 7---//
/* Υπολογισμός του πίνακα ανομοιομορφίας για τις φυσικοχημικές ιδιότητες*/
void Mesh3DScene::calc_dissimilarity_matrix_prop(Eigen::MatrixXf& arr_histogramms, Eigen::MatrixXf& diss_matrix, std::string name) {
	for (int i = 0; i < 369; i++) {
		for (int j = 0; j < 369; j++) {
			VectorXf v1 = arr_histogramms.row(i);
			VectorXf v2 = arr_histogramms.row(j);
			diss_matrix(i, j) = calc_error(v1, v2);
		}
	}
	float max_val = diss_matrix.maxCoeff();
	for (int i = 0; i < 369; i++) {
		for (int j = 0; j < 369; j++) {
			diss_matrix(i, j) /= max_val;
		}
	}
	ofstream myfile(name);
	if (myfile.is_open()) {
		for (int i = 0; i < 369; i++) {
			for (int j = 0; j < 369; j++) {
				float lambda = diss_matrix(i, j);
				myfile << lambda << " ";
			}
			myfile << "\n";
		}
		myfile.close();
	}
	else cout << "Unable to open file";
}
/* Υπολογισμός του πίνακα ανομοιομορφίας για όλες τις φυσικοχημικές ιδιότητες*/
void Mesh3DScene::calc_dissimilarity_matrix_prop_total(std::string name) {
	diss_properties_total = (diss_properties_col_0 + diss_properties_col_1 + diss_properties_col_2) / 3;
	float max_val = diss_properties_total.maxCoeff();
	for (int i = 0; i < 369; i++) {
		for (int j = 0; j < 369; j++) {
			diss_properties_total(i, j) /= max_val;
		}
	}
	ofstream myfile(name);
	if (myfile.is_open()) {
		for (int i = 0; i < 369; i++) {
			for (int j = 0; j < 369; j++) {
				float lambda = diss_properties_total(i, j);
				myfile << lambda << " ";
			}
			myfile << "\n";
		}
		myfile.close();
	}
	else cout << "Unable to open file";
}
/* Υπολογισμός του πίνακα ομοιομορφίας των πρωτεϊνών με συγκεκριμένο κατώφλι για τις φυσικοχημικές ιδιότητες*/
void Mesh3DScene::similar_prop(float Thres, Eigen::MatrixXf& diss_matrix, std::string name) {
	int rows_matrix = diss_matrix_prop_bool.rows();
	int cols_matrix = diss_matrix_prop_bool.cols();
	for (int i = 0; i < rows_matrix; i++) {
		for (int j = 0; j < rows_matrix; j++) {
			if (diss_matrix(i, j) <= Thres && i != j) {
				diss_matrix_prop_bool(i, j) = 1;
			}
		}
	}
	ofstream myfile(name);
	if (myfile.is_open()) {
		for (int i = 0; i < 369; i++) {
			for (int j = 0; j < 369; j++) {
				float lambda = diss_matrix_prop_bool(i, j);
				myfile << lambda << " ";
			}
			myfile << "\n";
		}
		myfile.close();
	}
	else cout << "Unable to open file";
}
/* Ταξινόμηση πρωτεϊνων*/
void Mesh3DScene::classification_protein_prop(Eigen::MatrixXf& diss_matrix, Eigen::VectorXf& c) {
	int teams[5] = { 44, 116, 161, 226, 368 };
	int size_c = 369;
	VectorXf c_correct = build_c_correct(teams, size_c);
	int rows_matrix = diss_matrix_prop_bool.rows();
	int cols_matrix = diss_matrix_prop_bool.cols();
	for (int i = 0; i < rows_matrix; i++) {
		VectorXf Rep = VectorXf::Zero(5);
		for (int j = 0; j < rows_matrix; j++) {
			if (diss_matrix_prop_bool(i, j) == 1) {
				for (int k = 0; k < 5; k++) {
					if (j <= teams[k]) {
						Rep[k]++;
						break;
					}
				}
			}
		}
		int class_prot = static_cast<int>(max_index(Rep));
		c(i) = class_prot;
	}
	VectorXf Rc = VectorXf::Zero(5);
	for (int i = 0; i < 369; i++) {
		if (c(i) == c_correct(i)) {
			Rc(static_cast<int>(c(i)))++;
		}
	}
	float error = 1 - Rc.sum() / 369;
	cout << "error classification: " << error << endl;
	cout << "classification: " << endl;
	print_VectorXf(c);
}
/* Εύρεση της πιο όμοιας πρωτεΐνης και προσθήκη σε διάνυσμα σύμφωνα με τις φυσικοχημικές ιδιότητες*/
void Mesh3DScene::find_similar_vector_prop(Eigen::VectorXf& Vec_similar, std::string propertie) {
	for (int i = 0; i < 41; i++) {
		similar_test_data_prop = VectorXf::Zero(369);
		VectorXf hist_obj_protein_testset_prop = VectorXf::Zero(HIST_SIZE);
		string name_test = to_string(i) + ".txt";
		string hist_test_prop = getBasePath() + "resources/histogram/properties_test_set/" + propertie + to_string(i) + ".txt";
		open_hist(hist_test_prop, hist_obj_protein_testset_prop);
		for (int j = 0; j < 369; j++) {
			VectorXf hist_obj_protein_dataset_prop = VectorXf::Zero(HIST_SIZE);
			string name_data = to_string(j) + ".txt";
			string hist_data_prop = getBasePath() + "resources/histogram/properties_dataset/" + propertie + to_string(j) + ".txt";
			open_hist(hist_data_prop, hist_obj_protein_dataset_prop);
			// Υπολογισμός Ομοιομορφίας
			float sim_prop = calc_error(hist_obj_protein_testset_prop, hist_obj_protein_dataset_prop);
			similar_test_data_prop(j) = sim_prop;
		}
		normalize_VectorXf(similar_test_data_prop);
		int index_min = min_index(similar_test_data_prop);
		Vec_similar(i) = index_min;
	}
}
/* Εύρεση της πιο όμοιας πρωτεΐνης και προσθήκη σε διάνυσμα για όλες τις φυσικοχημικές ιδιότητες*/
void Mesh3DScene::find_similar_vector_prop_total() {
	for (int i = 0; i < 41; i++) {
		similar_test_data_prop = VectorXf::Zero(369);
		VectorXf hist_obj_protein_testset_prop_0 = VectorXf::Zero(HIST_SIZE);
		VectorXf hist_obj_protein_testset_prop_1 = VectorXf::Zero(HIST_SIZE);
		VectorXf hist_obj_protein_testset_prop_2 = VectorXf::Zero(HIST_SIZE);
		string hist_test_prop_0 = getBasePath() + "resources/histogram/properties_test_set/0/" + to_string(i) + ".txt";
		string hist_test_prop_1 = getBasePath() + "resources/histogram/properties_test_set/1/" + to_string(i) + ".txt";
		string hist_test_prop_2 = getBasePath() + "resources/histogram/properties_test_set/2/" + to_string(i) + ".txt";
		open_hist(hist_test_prop_0, hist_obj_protein_testset_prop_0);
		open_hist(hist_test_prop_1, hist_obj_protein_testset_prop_1);
		open_hist(hist_test_prop_2, hist_obj_protein_testset_prop_2);
		for (int j = 0; j < 369; j++) {
			VectorXf hist_obj_protein_dataset_prop_0 = VectorXf::Zero(HIST_SIZE);
			VectorXf hist_obj_protein_dataset_prop_1 = VectorXf::Zero(HIST_SIZE);
			VectorXf hist_obj_protein_dataset_prop_2 = VectorXf::Zero(HIST_SIZE);
			string hist_data_prop_0 = getBasePath() + "resources/histogram/properties_dataset/0/" + to_string(j) + ".txt";
			string hist_data_prop_1 = getBasePath() + "resources/histogram/properties_dataset/1/" + to_string(j) + ".txt";
			string hist_data_prop_2 = getBasePath() + "resources/histogram/properties_dataset/2/" + to_string(j) + ".txt";
			open_hist(hist_data_prop_0, hist_obj_protein_dataset_prop_0);
			open_hist(hist_data_prop_1, hist_obj_protein_dataset_prop_1);
			open_hist(hist_data_prop_2, hist_obj_protein_dataset_prop_2);
			// Υπολογισμός Ομοιομορφίας
			float sim_prop_0 = calc_error(hist_obj_protein_testset_prop_0, hist_obj_protein_dataset_prop_0);
			float sim_prop_1 = calc_error(hist_obj_protein_testset_prop_1, hist_obj_protein_dataset_prop_1);
			float sim_prop_2 = calc_error(hist_obj_protein_testset_prop_2, hist_obj_protein_dataset_prop_2);
			similar_test_data_prop(j) = (sim_prop_0 + sim_prop_1 + sim_prop_2)/3;
		}
		normalize_VectorXf(similar_test_data_prop);
		int index_min = min_index(similar_test_data_prop);
		Vec_similar_protein_col_total(i) = index_min;
	}
}
/* Υπολογισμός ποσοστού ανάκτησης*/
void Mesh3DScene::retriaval_degree_prop(Eigen::VectorXf& Vec_similar, Eigen::VectorXf& c) {
	int size_c = 41;
	int teams_test[5] = { 4, 12, 17, 22, 40 };
	VectorXf c_correct_test = build_c_correct(teams_test, size_c);
	VectorXf c_test = VectorXf::Zero(size_c);
	for (int j = 0; j < Vec_similar.rows(); j++) {
		c_test[j] = c[Vec_similar[j]];
	}
	cout << "T: " << T << "->";
	VectorXf Rc = VectorXf::Zero(5);
	for (int j = 0; j < Vec_similar.rows(); j++) {
		if (c_test[j] == c_correct_test(j)) {
			Rc[c_test[j]] += 1;
		}
	}
	print_VectorXf(c_test);
	// Retrieval
	float retrieval = Rc.sum() / 41;
	cout << "retrieval:" << retrieval << endl;
}

//--- Draw ---//

void Mesh3DScene::draw() {
	//---Task 0---//
	if (TASK == 0) {
		if (m_style_flag & FLAG_SHOW_SOLID) m_model.draw(m_obj_col, SOLID);
	}
	//---Task 1---//
	if (TASK == 1) {
		if (m_style_flag & FLAG_SHOW_SOLID) m_model.draw(m_obj_col, SOLID);
		// physicochemical_properties column 0
		if (m_style_flag & FLAG_SHOW_SOLID) m_model_col_0.draw(m_obj_col, SOLID);
		vector<vvr::Triangle>& triangles0 = m_model_col_0.getTriangles();
		for (int i = 0; i < triangles0.size(); i++) {
			vvr::Triangle& t = triangles0[i];
			Triangle3D t3d(
				t.v1().x, t.v1().y, t.v1().z,
				t.v2().x, t.v2().y, t.v2().z,
				t.v3().x, t.v3().y, t.v3().z,
				colour_face_0[i]);
			t3d.draw();
		}
		// physicochemical_properties column 1
		if (m_style_flag & FLAG_SHOW_SOLID) m_model_col_1.draw(m_obj_col, SOLID);
		vector<vvr::Triangle>& triangles1 = m_model_col_1.getTriangles();
		for (int i = 0; i < triangles1.size(); i++) {
			vvr::Triangle& t = triangles1[i];
			Triangle3D t3d(
				t.v1().x, t.v1().y, t.v1().z,
				t.v2().x, t.v2().y, t.v2().z,
				t.v3().x, t.v3().y, t.v3().z,
				colour_face_1[i]);
			t3d.draw();
		}
		// physicochemical_properties column 2
		if (m_style_flag & FLAG_SHOW_SOLID) m_model_col_2.draw(m_obj_col, SOLID);
		vector<vvr::Triangle>& triangles2 = m_model_col_2.getTriangles();
		for (int i = 0; i < triangles2.size(); i++) {
			vvr::Triangle& t = triangles2[i];
			Triangle3D t3d(
				t.v1().x, t.v1().y, t.v1().z,
				t.v2().x, t.v2().y, t.v2().z,
				t.v3().x, t.v3().y, t.v3().z,
				colour_face_2[i]);
			t3d.draw();
		}
	}
	//---Task 2---//
	if (TASK == 2) {
		// Gauss
		if (m_style_flag & FLAG_SHOW_SOLID) m_model_gauss.draw(m_obj_col, SOLID);
		vector<vvr::Triangle>& triangles0 = m_model_gauss.getTriangles();
		for (int i = 0; i < triangles0.size(); i++) {
			vvr::Triangle& t = triangles0[i];
			Triangle3D t3d(
				t.v1().x, t.v1().y, t.v1().z,
				t.v2().x, t.v2().y, t.v2().z,
				t.v3().x, t.v3().y, t.v3().z,
				colour_K[i]);
			t3d.draw();
		}
		// Mean
		if (m_style_flag & FLAG_SHOW_SOLID) m_model_mean.draw(m_obj_col, SOLID);
		vector<vvr::Triangle>& triangles1 = m_model_mean.getTriangles();
		for (int i = 0; i < triangles1.size(); i++) {
			vvr::Triangle& t = triangles1[i];
			Triangle3D t3d(
				t.v1().x, t.v1().y, t.v1().z,
				t.v2().x, t.v2().y, t.v2().z,
				t.v3().x, t.v3().y, t.v3().z,
				colour_H[i]);
			t3d.draw();
		}
	}
	//---Task 3---//
	if (TASK == 3) {
		for (int i = 0; i < Graph.size(); i++) {
			dual_vertice vd = Graph[i];
			int val = floor(Graph[i].p * 255);
			if (val >= 190) {
				vvr::Colour c(val, 255, 0);
				Point3D p3d(vd.dual_v.x, vd.dual_v.y, vd.dual_v.z, c);
				p3d.draw();
			}
			else {
				vvr::Colour c(0, 0, 0);
				Point3D p3d(vd.dual_v.x, vd.dual_v.y, vd.dual_v.z, c);
				p3d.draw();
			}
			for (int j = 0; j < vd.count_neigh + 1; j++) {
				LineSeg3D line3d(vd.dual_v.x, vd.dual_v.y, vd.dual_v.z,
					Graph[vd.neigh_dual_vec[j]].dual_v.x, Graph[vd.neigh_dual_vec[j]].dual_v.y, Graph[vd.neigh_dual_vec[j]].dual_v.z,
					vvr::Colour::green);
				line3d.draw();
			}
		}
	}
	//---Task 31---//
	if (TASK == 31) {
		for (int i = 0; i < Graph.size(); i++) {
			dual_vertice vd = Graph[i];
			if (is_projection[i] == 1) {
				vvr::Colour c(255, 255, 255);
				Point3D p3d(vd.dual_v.x, vd.dual_v.y, vd.dual_v.z, c);
				p3d.draw();
			}
			else {
				vvr::Colour c(0, 0, 0);
				Point3D p3d(vd.dual_v.x, vd.dual_v.y, vd.dual_v.z, c);
				p3d.draw();
			}
			for (int j = 0; j < vd.count_neigh + 1; j++) {
				LineSeg3D line3d(vd.dual_v.x, vd.dual_v.y, vd.dual_v.z,
					Graph[vd.neigh_dual_vec[j]].dual_v.x, Graph[vd.neigh_dual_vec[j]].dual_v.y, Graph[vd.neigh_dual_vec[j]].dual_v.z,
					vvr::Colour::green);
				line3d.draw();
			}
		}
	}
	//---Task 4ii---//
	if (TASK == 42) {
		for (int i = -185; i < 184; i++) {
			for (int j = -185; j < 184; j++) {
				if ((i == -185 && j == 185) || (i == 0 && j == 0)) {
					vec v(i, j, 0);
					int save = static_cast<int>(dissimilarity_matrix_proteins(i + 185, j + 185) * 255);
					Colour c(255, 0, 0);
					Point3D p(v.x, v.y, v.z, c);
					p.draw();
					continue;
				}
				vec v(i, j, 0);
				int save = static_cast<int>(dissimilarity_matrix_proteins(i + 185, j + 185) * 255);
				Colour c(0, 255 - save, 0);
				Point3D p(v.x, v.y, v.z, c);
				p.draw();
			}
		}
	}
	//---Task 5---//
	if (TASK == 5) {
		for (int i = -185; i < 184; i++) {
			for (int j = -185; j < 184; j++) {
				if ((i == -185 && j == -185) || (i == 0 && j == 0)) {
					vec v(i, j, 0);
					int save = static_cast<int>(dissimilarity_matrix_proteins_bool(i + 185, j + 185) * 255);
					Colour c(255, 0, 0);
					Point3D p(v.x, v.y, v.z, c);
					p.draw();
					continue;
				}
				vec v(i, j, 0);
				int save = static_cast<int>(dissimilarity_matrix_proteins_bool(i + 185, j + 185) * 255);
				Colour c(0, save, 0);
				Point3D p(v.x, v.y, v.z, c);
				p.draw();
			}
		}
	}
	//---Task 7---//
	if (TASK == 7) {
		for (int i = -185; i < 184; i++) {
			for (int j = -185; j < 184; j++) {
				if ((i == -185 && j == -185) || (i == 0 && j == 0)) {
					vec v(i, j, 0);
					int save = static_cast<int>(diss_properties_col_2(i + 185, j + 185) * 255);
					Colour c(255, 0, 0);
					Point3D p(v.x, v.y, v.z, c);
					p.draw();
					continue;
				}
				vec v(i, j, 0);
				int save = static_cast<int>(diss_properties_col_2(i + 185, j + 185) * 255);
				Colour c(0, 255 - save, 0);
				Point3D p(v.x, v.y, v.z, c);
				p.draw();
			}
		}
	}
}

void Mesh3DScene::arrowEvent(ArrowDir dir, int modif)
{
	/*math::vec n = m_plane.normal;
	if (dir == UP) m_plane_d += 1;
	if (dir == DOWN) m_plane_d -= 1;
	else if (dir == LEFT) n = math::float3x3::RotateY(DegToRad(1)).Transform(n);
	else if (dir == RIGHT) n = math::float3x3::RotateY(DegToRad(-1)).Transform(n);
	m_plane = Plane(n.Normalized(), m_plane_d);*/
 
}

void Mesh3DScene::keyEvent(unsigned char key, bool up, int modif)
{
	Scene::keyEvent(key, up, modif);
	key = tolower(key);

	switch (key)
	{
	case 's': m_style_flag ^= FLAG_SHOW_SOLID; break;
	case 'w': m_style_flag ^= FLAG_SHOW_WIRE; break;
	case 'n': m_style_flag ^= FLAG_SHOW_NORMALS; break;
	case 'a': m_style_flag ^= FLAG_SHOW_AXES; break;
	case 'p': m_style_flag ^= FLAG_SHOW_PLANE; break;
	case 'b': m_style_flag ^= FLAG_SHOW_AABB; break;
	}
}
 
int main(int argc, char* argv[])
{
	try {
		return vvr::mainLoop(argc, argv, new Mesh3DScene);
	}
	catch (std::string exc) {
		cerr << exc << endl;
		return 1;
	}
	catch (...)
	{
		cerr << "Unknown exception" << endl;
		return 1;
	}
}