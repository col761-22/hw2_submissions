#include "LibGastonForSofia.h"
#include <string>
#include "stdio.h"
#include <cmath>

using namespace std;

void puti ( FILE *f, int i ) {
  char array[100];
  int k = 0;
  do {
    array[k] = ( i % 10 ) + '0';
    i /= 10;
    k++;
  }
  while ( i != 0 );
  do {
    k--;
    putc ( array[k], f );
  } while ( k );
}

bool callbackFunc(LibGastonDataRef data, const LibGastonGraph* graph)
{
    FILE* output = (FILE *) data;

    putc ( '#', output );
    putc ( ' ', output );
    puti ( output, graph->Support );
    putc ( '\n', output );

    static int counter = 0;
    counter++;
    putc ( 't', output );
    putc ( ' ', output );
    puti ( output, (int) counter );
    putc ( '\n', output );
    for ( int i = 0; i < graph->VertexCount; i++ ) {
        putc ( 'v', output );
        putc ( ' ', output );
        puti ( output, (int) i );
        putc ( ' ', output );
        puti ( output, (int) graph->Vertices[i] );
        putc ( '\n', output );
    }
    for ( int i = 0; i < graph->EdgeCount; i++ ) {
        putc ( 'e', output );
        putc ( ' ', output );
        puti ( output, (int) graph->Edges[i].From );
        putc ( ' ', output );
        puti ( output, (int) graph->Edges[i].To );
        putc ( ' ', output );
        puti ( output, (int) graph->Edges[i].Label );
        putc ( '\n', output );
    }
    putc( 'x', output );
    for( int i = 0; i < graph->Support; ++i ) {
        putc( ' ', output );
        puti( output, (int) graph->Objects[i] );
    }
    putc( '\n', output );
    
    return true;
}

int main(int argc, char* argv[])
{
    const char* gastonFilename = argv[1];
    const int support = atoi(argv[2]);
    int maxSize = ceil(atof(argv[3])); // [TODO] : [param] tune
    FILE* output = fopen(argv[4], "w");
    RunGaston(output, gastonFilename, support, callbackFunc, maxSize);
    fclose(output);
}