String CSV, filename;
Table t;
float maxAxis = 1000.0;
int nRows;
int maxIters;
int nElements = 0;
color[] elementColors = {};

void setup()
{
  // display in 3D on full screen
  fullScreen(P3D);

  // load table
  filename = "test";
  CSV = "data/" + filename + ".csv";
  t = loadTable(CSV, "header");
  
  background(40);
  
  // initial positioning
  nRows = t.getRowCount();
  for (TableRow row : t.findRows("0", "iter")) {
    pushMatrix();
    float x, y, z;
    x = row.getFloat("x")/1000.0 * width;
    y = row.getFloat("y")/1000.0 * height;
    z = row.getFloat("z");
    translate(x, y, z);

    // initial position defines the color of the sphere
    elementColors = append(elementColors, (color(x/maxAxis*255, y/maxAxis*255, z/maxAxis*255)));
    fill(elementColors[nElements]);
    stroke(1, 50);
    strokeWeight(1);
    sphere(row.getFloat("m")/10.0); // mass defines radius
    popMatrix();
    nElements++;
  }
  maxIters = nRows / nElements;
}

int iter = 1;
void draw()
{
  // end the visualization or restart it from the beginning
  if (iter == maxIters){
    iter = 0;
    //exit(); // uncomment if you want the visualization to exit after one iter
  }
  
  // update visualization
  background(40);
  // draw lines
  stroke(200, 100);
  // up left
  line(0, 0, 0, 
       0, 0, -1000);
  // up right
  line(width, 0, 0, 
       width, 0, -1000);
  // down left
  line(0, height, 0, 
       0, height, -1000);
  // down right
  line(width, height, 0, 
       width, height, -1000);
  // lines in the back (the rectangle)
  line(0, 0, -1000, 
       0, height, -1000);
  line(0, 0, -1000, 
       width, 0, -1000);
  line(width, height, -1000, 
       0, height, -1000);
  line(width, height, -1000, 
       width, 0, -1000);
  
  
  for (TableRow row : t.findRows(str(iter), "iter")) {
    pushMatrix();
    float x, y, z;
    x = row.getFloat("x")/1000.0 * width;
    y = row.getFloat("y")/1000.0 * height;
    z = row.getFloat("z");

    translate(x, y, z);
    fill(elementColors[row.getInt("ix")]);

    stroke(1, 50);
    strokeWeight(1);
    sphere(row.getFloat("m"));
    popMatrix();
  }
  iter++;
  delay(10);
}
