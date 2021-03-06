import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import com.sun.java.swing.*;

/**
 * This class shows up a dialog having the size options for printing
 */
public class PrintDlg extends JDialog 
implements ActionListener, ItemListener {
  private ClogDisplay parent;
  private JFrame printFrame;
  
  private MyTextField widthTxField, heightTxField;
  private JRadioButton cmRB, mmRB, inRB, ptRB;
 
  private double nonPrintGap;
  private int finalWidth, finalHeight, maxPageW, maxPageH, dpi;
  private Dimension pageDim;
  private PrintJob pjob;
  
  public PrintDlg (ClogDisplay p) {
    super (p, "Print Options", true);
    parent = p;
    setup ();
  }
  
  //setup methods----------------------------------------------------------
  private void setup () {setupPanel ();}
  
  private void setupPanel () {
    Container conPane = this.getContentPane ();
    
    conPane.setLayout (new GridBagLayout ());
    
    GridBagConstraints con = new GridBagConstraints ();
    con.anchor = GridBagConstraints.WEST;
    
    conPane.add (new JLabel ("width"), con);
    
    con.gridx = 1;
    conPane.add (widthTxField = new MyTextField ("", 10, true), con);
    
    con.gridx = 0; con.gridy = 1;
    conPane.add (new JLabel ("height"), con);
    
    con.gridx = 1;
    conPane.add (heightTxField = new MyTextField ("", 10, true), con);
    
    //Attach radio buttons for dimensions
    ButtonGroup group = new ButtonGroup ();
    
    con.gridx = 2; con.gridy = 0; 
    conPane.add (cmRB = new JRadioButton ("cm", false), con);
    cmRB.addItemListener (this); group.add (cmRB);
    
    con.gridy = 1;
    conPane.add (mmRB = new JRadioButton ("mm", false), con);
    mmRB.addItemListener (this); group.add (mmRB);

    con.gridy = 2;
    conPane.add (inRB = new JRadioButton ("inches", false), con);
    inRB.addItemListener (this); group.add (inRB);
    
    con.gridy = 3;
    conPane.add (ptRB = new JRadioButton ("points", false), con);
    ptRB.addItemListener (this); group.add (ptRB);

    con.gridx = 0;
    conPane.add (new MyButton ("Ok", this), con);
    
    con.gridx = 1;
    //This has been deliberately left out. In ClogDisplay we request a Print Dialog
    //and JVM gives us one. Then we are asked to accept or cancel printing. If 
    //we accept this dialog (PrintDlg) opens up. At this stage printing cannot
    //be canceled as the JVM has already written the file, so having a Cancel
    //Printing button does not make sense
    // conPane.add (new MyButton ("Cancel Printing", this), con);
    
    pack ();
    setSize (getMinimumSize ());
    setResizable (false);
  }
  //end of setup methods--------------------------------------------------------------
  
  //event handler methods-------------------------------------------------------------
  //events generated by buttons and radio buttons are caught

  /**
   * event handler for events generated by radio buttons
   */
  public void itemStateChanged (ItemEvent e) {
    if (e.getStateChange () == ItemEvent.DESELECTED) return;
    
    Object o = e.getItem ();
    
    if (o == mmRB) {width (toMm (maxPageW)); height (toMm (maxPageH));}
    else if (o == cmRB) {width (toCm (maxPageW)); height (toCm (maxPageH));}
    else if (o == inRB) {width (toInches (maxPageW)); height (toInches (maxPageH));}
    else if (o == ptRB) {width (maxPageW); height (maxPageH);}
  }

  /**
   * event handler for events generated by buttons
   */
  public void actionPerformed (ActionEvent evt) {
    if (evt.getActionCommand ().equals ("Ok")) {
      setVisible (false);
      
      double w, h;
      
      double wVal = (new Double (widthTxField.getText ())).doubleValue ();
      double hVal = (new Double (heightTxField.getText ())).doubleValue ();
      
      if (cmRB.isSelected ()) {w = fromCm (wVal); h = fromCm (hVal);}
      else if (mmRB.isSelected ()) {w = fromMm (wVal); h = fromMm (hVal);}
      else if (inRB.isSelected ()) {w = fromInches (wVal); h = fromInches (hVal);}
      else {w = wVal; h = hVal;}
      
      finalWidth = (int)Math.rint (w);
      finalHeight = (int)Math.rint (h);
      
      //Final Dimensions of the upshot data
      if (finalWidth > maxPageW) {
	JOptionPane.showMessageDialog (null, "Width >  Max Print Width. Printing may not work.", 
				       "width > max print width", JOptionPane.WARNING_MESSAGE);
	//finalWidth = maxPageW;
      }
      if (finalHeight > maxPageH) {
	JOptionPane.showMessageDialog (null, "Height >  Max Print Height. Printing may not work.", 
				       "height > max print height", JOptionPane.WARNING_MESSAGE);
	//finalHeight = maxPageH;
      }
      
      Graphics pg = pjob.getGraphics (); if (pg == null) return;
      
      //Need to set the font or else JVM crashes
      pg.setFont (parent.printFont); 
      FontMetrics fm = getToolkit ().getFontMetrics (pg.getFont ());
      
      //Center image
      int hgap = (int)Math.rint (nonPrintGap + (maxPageW - finalWidth) / 2.0);
      int vgap = (int)Math.rint (nonPrintGap + (maxPageH - finalHeight) / 2.0);
      
      //Draw border
      pg.setColor (Color.black);
      //      pg.drawRect (hgap - 1, vgap - 1, finalWidth + 1, finalHeight + 1);
      pg.drawString ("Logfile:" + parent.parent.logFileName, hgap, vgap + fm.getHeight () - 
		     fm.getDescent () - 1);
      // pg.drawRect (hgap, vgap + fm.getHeight (), finalWidth - 1, finalHeight - fm.getHeight () - 
      //   1);
      
      int remWt = finalWidth, remHt = finalHeight - fm.getHeight ();
	 
      if (printFrame instanceof ClogDisplay) {
	//StateButtons
	int sBx = hgap, sBy = vgap + fm.getHeight ();
	int sBHt = parent.stateButtons.print (pg, sBx, sBy, remWt, remHt); 
	 
	//Canvas
	int cx = hgap, cy = sBy + sBHt; remHt = remHt - sBHt; 
	new PrintCanvas (parent.canvas, pg, cx, cy, remWt, remHt, dpi);
      }
      else if (printFrame instanceof Histwin) {
	Histwin win = (Histwin)printFrame;
	
	//Print heading stuff
	int wx = hgap, wy = vgap + fm.getHeight ();
	int wBHt = win.canvas.print (pg, wx, wy, remWt, remHt);
	
	//Canvas
	int cx = hgap, cy = wy + wBHt; remHt = remHt - wBHt;
	new PHistCanvas (win.canvas, pg, cx, cy, remWt, remHt, dpi);
      }
          
      //flush page from printer when done
      pg.dispose (); 
      pjob.end ();
    }
  }
  //end of event handler methods-----------------------------------------------------------
  
  /**
   * This function is called when the user selects Ok to print
   * Options for size and printing commands are given.
   */
  void reset (PrintJob pj, JFrame frame) {
    pjob = pj;
    printFrame = frame;
    pageDim = pjob.getPageDimension ();
    dpi = getToolkit ().getScreenResolution ();//pjob.getPageResolution ();
    
    //Account for the non printing area (typically 1/4 th of an inch at all four sides)
    nonPrintGap = (double)0.25 * dpi;
    
    maxPageW = pageDim.width - (int)Math.rint (2 * nonPrintGap);
    maxPageH = pageDim.height - (int)Math.rint (2 * nonPrintGap);
    
    inRB.setSelected (true);
    
    width (toInches (maxPageW));
    height (toInches (maxPageH));
    
    setVisible (true);
  }
  
  /**
   * Functions to put the string in the text fields
   */
  private void width (double w) {widthTxField.setText (Float.toString ((float)w));}
  private void height (double h) {heightTxField.setText (Float.toString ((float)h));}

  /**
   * Functions to convert
   */
  private double toInches (double dots) {return (double)(dots * 1.0 / dpi); }
  private double toCm (double dots) {return (double)(toInches (dots) * 2.54);}
  private double toMm (double dots) {return toCm (dots) * 10;}
  private double fromInches (double inches) {return inches * dpi;}
  private double fromCm (double cm) {return (double)(fromInches (cm) / 2.54);}
  private double fromMm (double mm) {return (fromCm (mm) / 10);}
}



