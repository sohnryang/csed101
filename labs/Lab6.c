#include <stdio.h>

int main() {
  FILE *infile = fopen("score.txt", "r");
  if (infile == NULL) {
    printf("error: could not open file");
    return 1;
  }
  FILE *outfile = fopen("report.txt", "w");

  int student_no, midterm_score, final_score, student_count = 0;
  float score_sum;
  fprintf(outfile, "===========================\n");
  fprintf(outfile, "   학번       총점(학점)\n");
  fprintf(outfile, "===========================\n");
  while (fscanf(infile, "%d %d %d", &student_no, &midterm_score,
                &final_score) != EOF) {
    float aggregated_score = 0.4 * midterm_score + 0.6 * final_score;
    char grade;
    if (aggregated_score >= 90)
      grade = 'A';
    else if (aggregated_score >= 80)
      grade = 'B';
    else if (aggregated_score >= 70)
      grade = 'C';
    else if (aggregated_score >= 60)
      grade = 'D';
    else
      grade = 'F';
    fprintf(outfile, " %d      %.1f (%c)\n", student_no, aggregated_score,
            grade);
    score_sum += aggregated_score;
    student_count++;
  }
  fprintf(outfile, "===========================\n");
  fprintf(outfile, "   평균        %.1f\n", score_sum / student_count);
  fprintf(outfile, "===========================\n");

  fclose(infile);
  fclose(outfile);
  return 0;
}
