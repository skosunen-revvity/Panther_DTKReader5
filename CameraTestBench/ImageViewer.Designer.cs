namespace PKI.Puncher.CameraTestBench {
  partial class ImageViewer {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing) {
      if (disposing && (components != null)) {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent() {
        this.cmdClose = new System.Windows.Forms.Button();
        this.pictureBox1 = new System.Windows.Forms.PictureBox();
        this.cmdSave = new System.Windows.Forms.Button();
        this.label1 = new System.Windows.Forms.Label();
        this.txtFilename = new System.Windows.Forms.TextBox();
        this.cmdBrowse = new System.Windows.Forms.Button();
        this.lblDrawingSurface = new System.Windows.Forms.Label();
        ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
        this.SuspendLayout();
        // 
        // cmdClose
        // 
        this.cmdClose.DialogResult = System.Windows.Forms.DialogResult.Cancel;
        this.cmdClose.Location = new System.Drawing.Point(584, 168);
        this.cmdClose.Name = "cmdClose";
        this.cmdClose.Size = new System.Drawing.Size(96, 20);
        this.cmdClose.TabIndex = 0;
        this.cmdClose.Text = "Close";
        this.cmdClose.UseVisualStyleBackColor = true;
        this.cmdClose.Click += new System.EventHandler(this.cmdClose_Click);
        // 
        // pictureBox1
        // 
        this.pictureBox1.Location = new System.Drawing.Point(8, 8);
        this.pictureBox1.Name = "pictureBox1";
        this.pictureBox1.Size = new System.Drawing.Size(488, 152);
        this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
        this.pictureBox1.TabIndex = 1;
        this.pictureBox1.TabStop = false;
        // 
        // cmdSave
        // 
        this.cmdSave.Location = new System.Drawing.Point(504, 168);
        this.cmdSave.Name = "cmdSave";
        this.cmdSave.Size = new System.Drawing.Size(75, 20);
        this.cmdSave.TabIndex = 2;
        this.cmdSave.Text = "Save";
        this.cmdSave.UseVisualStyleBackColor = true;
        this.cmdSave.Click += new System.EventHandler(this.cmdSave_Click);
        // 
        // label1
        // 
        this.label1.AutoSize = true;
        this.label1.Location = new System.Drawing.Point(8, 172);
        this.label1.Name = "label1";
        this.label1.Size = new System.Drawing.Size(49, 13);
        this.label1.TabIndex = 3;
        this.label1.Text = "Filename";
        // 
        // txtFilename
        // 
        this.txtFilename.Location = new System.Drawing.Point(64, 168);
        this.txtFilename.Name = "txtFilename";
        this.txtFilename.Size = new System.Drawing.Size(408, 20);
        this.txtFilename.TabIndex = 4;
        // 
        // cmdBrowse
        // 
        this.cmdBrowse.Location = new System.Drawing.Point(472, 168);
        this.cmdBrowse.Name = "cmdBrowse";
        this.cmdBrowse.Size = new System.Drawing.Size(24, 20);
        this.cmdBrowse.TabIndex = 5;
        this.cmdBrowse.Text = "...";
        this.cmdBrowse.UseVisualStyleBackColor = true;
        this.cmdBrowse.Click += new System.EventHandler(this.cmdBrowse_Click);
        // 
        // lblDrawingSurface
        // 
        this.lblDrawingSurface.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
        this.lblDrawingSurface.Location = new System.Drawing.Point(512, 56);
        this.lblDrawingSurface.Name = "lblDrawingSurface";
        this.lblDrawingSurface.Size = new System.Drawing.Size(112, 48);
        this.lblDrawingSurface.TabIndex = 6;
        // 
        // ImageViewer
        // 
        this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
        this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
        this.CancelButton = this.cmdClose;
        this.ClientSize = new System.Drawing.Size(692, 207);
        this.ControlBox = false;
        this.Controls.Add(this.lblDrawingSurface);
        this.Controls.Add(this.cmdBrowse);
        this.Controls.Add(this.txtFilename);
        this.Controls.Add(this.label1);
        this.Controls.Add(this.cmdSave);
        this.Controls.Add(this.pictureBox1);
        this.Controls.Add(this.cmdClose);
        this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
        this.Name = "ImageViewer";
        this.Text = "ImageViewer";
        ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
        this.ResumeLayout(false);
        this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Button cmdClose;
    private System.Windows.Forms.PictureBox pictureBox1;
    private System.Windows.Forms.Button cmdSave;
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.TextBox txtFilename;
    private System.Windows.Forms.Button cmdBrowse;
    private System.Windows.Forms.Label lblDrawingSurface;
  }
}