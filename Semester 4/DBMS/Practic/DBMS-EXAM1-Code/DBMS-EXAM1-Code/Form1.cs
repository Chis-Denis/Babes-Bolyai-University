using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Configuration;

namespace DBMS_EXAM1_Code
{
    public partial class Form1 : Form
    {
        private DataSet dataSet = new DataSet();
        private SqlDataAdapter daDentalCabinet;
        private SqlDataAdapter daDentists;
        private BindingSource bsDentalCabinet = new BindingSource();
        private BindingSource bsDentists = new BindingSource();
        private SqlConnection conn;

        public Form1()
        {
            InitializeComponent();
            Load += Form1_Load;
            dgvDentalCabinet.SelectionChanged += DgvDentalCabinet_SelectionChanged;
            btnSave.Click += BtnSave_Click;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string connStr = ConfigurationManager.ConnectionStrings["DBMS_EXAM1"].ConnectionString;
            conn = new SqlConnection(connStr);

            // DentalCabinet
            daDentalCabinet = new SqlDataAdapter("SELECT * FROM DentalCabinet", conn);
            SqlCommandBuilder cbDentalCabinet = new SqlCommandBuilder(daDentalCabinet);
            daDentalCabinet.Fill(dataSet, "DentalCabinet");

            // Dentists
            daDentists = new SqlDataAdapter("SELECT * FROM Dentist", conn);
            SqlCommandBuilder cbDentists = new SqlCommandBuilder(daDentists);
            daDentists.Fill(dataSet, "Dentist");

            // DataRelation
            dataSet.Relations.Add("Cabinet_Dentists",
                dataSet.Tables["DentalCabinet"].Columns["DentalCabinetID"],
                dataSet.Tables["Dentist"].Columns["DentalCabinetID"]);

            // Bindings
            bsDentalCabinet.DataSource = dataSet;
            bsDentalCabinet.DataMember = "DentalCabinet";
            dgvDentalCabinet.DataSource = bsDentalCabinet;

            bsDentists.DataSource = bsDentalCabinet;
            bsDentists.DataMember = "Cabinet_Dentists";
            dgvDentists.DataSource = bsDentists;
        }

        private void DgvDentalCabinet_SelectionChanged(object sender, EventArgs e)
        {
            // The BindingSource handles filtering automatically
        }

        private void BtnSave_Click(object sender, EventArgs e)
        {
            try
            {
                daDentalCabinet.Update(dataSet, "DentalCabinet");
                daDentists.Update(dataSet, "Dentist");
                MessageBox.Show("Changes saved successfully.");
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error saving changes: " + ex.Message);
            }
        }
    }
}
