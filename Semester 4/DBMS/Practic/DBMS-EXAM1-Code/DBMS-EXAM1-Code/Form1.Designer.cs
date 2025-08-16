namespace DBMS_EXAM1_Code
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;
        private System.Windows.Forms.DataGridView dgvDentalCabinet;
        private System.Windows.Forms.DataGridView dgvDentists;
        private System.Windows.Forms.Button btnSave;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.dgvDentalCabinet = new System.Windows.Forms.DataGridView();
            this.dgvDentists = new System.Windows.Forms.DataGridView();
            this.btnSave = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.dgvDentalCabinet)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dgvDentists)).BeginInit();
            this.SuspendLayout();
            // 
            // dgvDentalCabinet
            // 
            this.dgvDentalCabinet.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvDentalCabinet.Location = new System.Drawing.Point(12, 12);
            this.dgvDentalCabinet.Name = "dgvDentalCabinet";
            this.dgvDentalCabinet.Size = new System.Drawing.Size(300, 350);
            this.dgvDentalCabinet.TabIndex = 0;
            // 
            // dgvDentists
            // 
            this.dgvDentists.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvDentists.Location = new System.Drawing.Point(330, 12);
            this.dgvDentists.Name = "dgvDentists";
            this.dgvDentists.Size = new System.Drawing.Size(450, 350);
            this.dgvDentists.TabIndex = 1;
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(330, 380);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(120, 40);
            this.btnSave.TabIndex = 2;
            this.btnSave.Text = "Save Changes";
            this.btnSave.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.dgvDentalCabinet);
            this.Controls.Add(this.dgvDentists);
            this.Controls.Add(this.btnSave);
            this.Name = "Form1";
            this.Text = "Dental Cabinets Master/Detail";
            ((System.ComponentModel.ISupportInitialize)(this.dgvDentalCabinet)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dgvDentists)).EndInit();
            this.ResumeLayout(false);
        }

        #endregion
    }
}

