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

namespace Lab_1
{
    public partial class Form1 : Form
    {
        SqlConnection myConnection;
        SqlDataAdapter manufacturerDA, aircraftModelDA;
        DataSet myDataSet;
        SqlCommandBuilder myCommandBuilder;
        BindingSource aircraftModelBindingSource, manufacturerBindingSource;
        
        string manufacturerTable = ConfigurationManager.AppSettings["parentTable"];
        string aircraftModelTable = ConfigurationManager.AppSettings["childTable"];
        string manufacturerFK = ConfigurationManager.AppSettings["childFK"];
        string manufacturerPK = ConfigurationManager.AppSettings["parentFKCol"];

        public Form1()
        {
            InitializeComponent();
            LoadData();
        }

        private string GetConnectionString()
        {
            return ConfigurationManager.ConnectionStrings["connectionString"].ConnectionString;
        }

        private void LoadData()
        {
            try
            {
                myConnection = new SqlConnection(GetConnectionString());
                myDataSet = new DataSet();

                // Load Manufacturers and Aircraft Models
                manufacturerDA = new SqlDataAdapter("SELECT * FROM " + manufacturerTable + " ORDER BY Name", myConnection);
                aircraftModelDA = new SqlDataAdapter("SELECT * FROM " + aircraftModelTable + " ORDER BY ModelName", myConnection);
                
                // Enable update capabilities
                myCommandBuilder = new SqlCommandBuilder(aircraftModelDA);
                SqlCommandBuilder manufacturerCommandBuilder = new SqlCommandBuilder(manufacturerDA);

                manufacturerDA.Fill(myDataSet, manufacturerTable);
                aircraftModelDA.Fill(myDataSet, aircraftModelTable);

                // Create relation between Manufacturer and AircraftModel
                DataRelation myDataRelation = new DataRelation("FK_AircraftModel_Manufacturer",
                    myDataSet.Tables[manufacturerTable].Columns[manufacturerPK],
                    myDataSet.Tables[aircraftModelTable].Columns[manufacturerFK]);
                myDataSet.Relations.Add(myDataRelation);

                // Set up data bindings
                manufacturerBindingSource = new BindingSource();
                aircraftModelBindingSource = new BindingSource();

                manufacturerBindingSource.DataSource = myDataSet;
                manufacturerBindingSource.DataMember = manufacturerTable;

                aircraftModelBindingSource.DataSource = manufacturerBindingSource;
                aircraftModelBindingSource.DataMember = "FK_AircraftModel_Manufacturer";

                // Bind to grids
                ManufacturerGrid.DataSource = manufacturerBindingSource;
                AircraftModelGrid.DataSource = aircraftModelBindingSource;

                // Configure grid columns
                ConfigureGridColumns();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error loading data: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void ConfigureGridColumns()
        {
            // Configure Manufacturer grid
            if (ManufacturerGrid.Columns.Count > 0)
            {
                ManufacturerGrid.Columns["ManufacturerID"].Visible = false;
                ManufacturerGrid.Columns["Name"].HeaderText = "Manufacturer Name";
                ManufacturerGrid.Columns["Country"].HeaderText = "Country";
            }

            // Configure Aircraft Model grid
            if (AircraftModelGrid.Columns.Count > 0)
            {
                AircraftModelGrid.Columns["ModelID"].Visible = false;
                AircraftModelGrid.Columns["ManufacturerID"].Visible = false;
                AircraftModelGrid.Columns["ModelName"].HeaderText = "Model Name";
                AircraftModelGrid.Columns["YearOfProduction"].HeaderText = "Production Year";
            }
        }

        private void SaveButton_Click(object sender, EventArgs e)
        {
            try
            {
                // Save changes to both tables
                manufacturerDA.Update(myDataSet, manufacturerTable);
                aircraftModelDA.Update(myDataSet, aircraftModelTable);
                MessageBox.Show("Changes saved successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error saving changes: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void RefreshButton_Click(object sender, EventArgs e)
        {
            try
            {
                myDataSet.Clear();
                LoadData();
                MessageBox.Show("Data refreshed successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error refreshing data: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
} 