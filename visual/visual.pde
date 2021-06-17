String CSV, filename;
Table t;
float maxAxis = 1000.0;
int nRows;
int maxIters;
int nElements = 0;
color[] elementColors = {};

void setup()
{
  // Display in 3D, x & y borders
  // TODO mogoče bi mogl scalat na full screen display pa pomnožit vrednosti
  size(1000, 1000, P3D);

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
    x = row.getFloat("x"); // change code
    y = row.getFloat("y");
    z = row.getFloat("z");
    // println(x, y, z, row.getFloat("m")); // debug
    translate(x, y, z);
    elementColors = append(elementColors, (color(x/maxAxis*255, y/maxAxis*255, z/maxAxis*255)));
    fill(elementColors[nElements]);
    noStroke();
    sphere(row.getFloat("m")/10.0); // mass defines radius; change code
    popMatrix();
    nElements++;
  }
  maxIters = nRows / nElements;
}

int iter = 1;
void draw()
{
  // end the visualization
  if (iter == maxIters){
    iter = 0;
    //exit();
  }
  
  // update visualization
  background(40);
  for (TableRow row : t.findRows(str(iter), "iter")) {
    pushMatrix();
    float x, y, z;
    x = row.getFloat("x"); // change code
    y = row.getFloat("y");
    z = row.getFloat("z");
    // println(x, y, z); // debug
    translate(x, y, z);
    elementColors = append(elementColors, (color(x/maxAxis*255, y/maxAxis*255, z/maxAxis*255)));
    fill(elementColors[row.getInt("ix")]);
    noStroke();
    sphere(row.getFloat("m")); // mass defines radius
    popMatrix();
  }
  iter++;
  delay(10);
}
