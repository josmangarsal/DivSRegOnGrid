#include <iostream>
#include <queue>

using namespace std;

int main() {
	int m = 11;
	int n = 3;
	int teller = 1;

	double div = m - 1;

	queue<double*> grid;
	double* initialGrid = new double[n];
	for (int i = 0; i < n; i++)
		initialGrid[i] = 0;
	initialGrid[1] = div;
	grid.push(initialGrid);

	double* xTemp = new double[n];
	for (int i = 0; i < n; i++)
		xTemp[i] = 0;

	for (int i2 = 0; i2 <= m - 2; i2++) {

		xTemp[1] = i2;
		for (int i3 = 0; i3 <= m - i2 - 1; i3++) {
			xTemp[2] = i3;
			// ...
			double suma = 0;
			for (int i = 1; i < n; i++)
				suma = suma + xTemp[i];
			double i1 = div - suma;
			xTemp[0] = i1;

			double* x = new double[n];
			for(int i = 0; i < n; i++){
				x[i] = xTemp[i];
			}
			grid.push(x);
			teller++;
		}
	}

	delete[] xTemp;

	cout << "Teller = " << teller << endl;

	queue<double*> finalGrid;
	while (!grid.empty()) {
		double* g = grid.front();
		grid.pop();
		for (int i = 0; i < n; i++) {
			g[i] = g[i] / div;
		}
		finalGrid.push(g);
	}

	while (!finalGrid.empty()) {
		for (int i = 0; i < n; i++) {
			cout << finalGrid.front()[i] << " ";
		}
		cout << endl;

		delete[] finalGrid.front();

		finalGrid.pop();
	}

	return 0;
}
