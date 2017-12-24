#include <stdlib.h>

//void gen_code(tree *decl, tree *stmt_seq)
void gen_code_decl(tree *ptr, int tcount);
void gen_code_stmt_seq(tree *ptr, int tcount);
void gen_code_expr(tree *ptr);

int count = 0;

tree* make_node(tree *decl_list, tree *stmt_list){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = prog_node;
   ptr->children.left = decl_list;
   ptr->children.center = stmt_list;

   return ptr;
}

tree* make_decl(char *name, int type, tree *next){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = decl_node;
   ptr->children.sval = name;
   ptr->children.val = type;
   ptr->children.left = next;
   return ptr;
}

tree* make_stmt_seq(tree* stmt, tree* next){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = stmt_seq_node;
   ptr->children.left = stmt;
   ptr->children.center = next;
//   if(next == NULL) ptr->children.center = next;
//   else{
//      next->children.center = ptr;
//      ptr->children.center = NULL;
//   }
//   ptr->children.center = NULL;
//   next->children.center = ptr;
//   ptr->children.center = NULL;
   return ptr;
}

tree* make_stmt(tree* nstmt){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = stmt_node;
   ptr->children.left = nstmt;
   return ptr;
}

tree* make_asgn_rd(char* name){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = asgn_rd_node;
   ptr->children.sval = name;
   return ptr;
}

tree* make_asgn(char *name, tree* expr_list){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = asgn_node;
   ptr->children.sval = name;
   ptr->children.left = expr_list;
   return ptr;
}

tree* make_if(tree* expr_list, tree* stmt_seq, tree* elseClause){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = if_node;
   ptr->children.left = expr_list;
   ptr->children.center = stmt_seq;
   ptr->children.right = elseClause;
   return ptr;
}

tree* make_else(tree* stmt_seq){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = else_node;
   ptr->children.left = stmt_seq;
   return ptr;
}

tree* make_while(tree* expr_list, tree* stmt_seq){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = while_node;
   ptr->children.left = expr_list;
   ptr->children.center = stmt_seq;
   return ptr;
}

tree* make_write_int(tree* expr_list){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = write_int_node;
   ptr->children.left = expr_list;
   return ptr;
}

tree* make_expr(tree* expr){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = expr_node;
   ptr->children.left = expr;
   return ptr;
}

tree* make_expr2(tree* expr, char* sval, tree* expr2){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = expr2_node;
   ptr->children.left = expr;
   ptr->children.sval = sval;
   ptr->children.center = expr2;
   return ptr;
}

tree* make_expr_s(tree* s_expr){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = expr_s_node;
   ptr->children.left = s_expr;
   return ptr;
}

tree* make_expr_s2(tree* s_expr, char* sval, tree* s_expr2){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = expr_s2_node;
   ptr->children.left = s_expr;
   ptr->children.sval = sval;
   ptr->children.center = s_expr2;
   return ptr;
}

tree* make_term2(tree* factor1, char* sval, tree* factor2){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = term2_node;
   ptr->children.left = factor1;
   ptr->children.sval = sval;
   ptr->children.center = factor2;
   return ptr;
}

tree* make_term(tree* factor1){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = term1_node;
   ptr->children.left = factor1;
   return ptr;
}

tree* make_factor_num(int val){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = factor_num_node;
   ptr->val = val;
   return ptr;
}
tree* make_factor_boollit(char * sval){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = factor_boollit_node;
   ptr->sval = sval;
   return ptr;
}
tree* make_factor_ident(char * ident){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = factor_ident_node;
   ptr->sval = ident;
   return ptr;
}

tree* make_factor_expr(tree* expr_list){
   tree *ptr = malloc(sizeof(tree));
   ptr->nodetype = factor_expr_node;
   ptr->children.left = expr_list;
   return ptr;
}

void print_tabs(int tcount){
   int ix=0;
   for(ix; ix<tcount; ix++){
      printf("\t");
   }
}



void gen_code_factor(tree *ptr){
   if(ptr == NULL) return;
   if(ptr->nodetype == factor_ident_node) printf("%s", ptr->sval);
   if(ptr->nodetype == factor_num_node) {
      int tval = ptr->val;
//      printf("%d", tval);
      if(tval < 0) printf("Error, Integer overflow!");
      else printf("%d", ptr->val);
   }
      

   if(ptr->nodetype == factor_boollit_node) {      
      printf("Hello world!");
      printf("%s", ptr->sval);
   
   }
   if(ptr->nodetype == factor_expr_node) {
      printf("(");
      gen_code_expr(ptr->children.left);
      printf(")");
   }
}


void gen_code_term(tree *ptr){
   if(ptr==NULL) return;
   int test = 0;
   gen_code_factor(ptr->children.left);
   if(ptr->children.center != NULL){
      if(ptr->children.left->nodetype == 18){
         char*varname = ptr->children.left->sval;
	 struct my_struct *s;
	 s = find_sym(varname);
	 if(s == NULL) printf(" Error, undeclared variable: %s\n", varname);
	 else if(s->type == 1) test = 1;
      }
      else if(ptr->children.left->nodetype != 16){
         test = 1;
      }
      if(ptr->children.center->nodetype == 18){
         char*varname2 = ptr->children.center->sval;
	 struct my_struct *s2;
	 s2 = find_sym(varname2);
	 if(s2 == NULL) printf(" Error, undeclared variable: %s\n", varname2);
	 else if(s2->type == 1) test = 1;
      }
      else if(ptr->children.center->nodetype != 16){
         test = 1;
      }
      if(test==0){
         printf(" %s ", ptr->sval);
         gen_code_factor(ptr->children.center);
	 }
      else printf(" Error, operands of \"%s\" must be integers", ptr->children.sval);
   }
}

void gen_code_expr_s(tree *ptr){
   if(ptr==NULL) return;
   int test = 0;
   gen_code_term(ptr->children.left);
   if(ptr->children.center != NULL){
      if(ptr->children.left->children.left->nodetype == 18){
         char*varname = ptr->children.left->children.left->sval;
	 struct my_struct *s;
	 s = find_sym(varname);
	 if(s == NULL) printf(" Error, undeclared variable: %s\n", varname);
	 else if(s->type == 1) test = 1;
      }
      else if(ptr->children.left->children.left->nodetype != 16){
         test = 1;
      }
      if(ptr->children.center->children.left->nodetype == 18){
         char*varname2 = ptr->children.center->children.left->sval;
	 struct my_struct *s2;
	 s2 = find_sym(varname2);
	 if(s2 == NULL) printf(" Error, undeclared variable: %s\n", varname2);
	 else if(s2->type == 1) test = 1;
      }
      else if(ptr->children.center->children.left->nodetype != 16){
         test = 1;
      }
      if(test==0){
         printf(" %s ", ptr->children.sval);
         gen_code_term(ptr->children.center);
	 }
      else printf(" Error, operands of \"%s\" must be integers", ptr->children.sval);
   }
}

void gen_code_expr(tree *ptr){
   if(ptr==NULL) return;
     gen_code_expr_s(ptr->children.left);
   if(ptr->children.center != NULL){
      printf(" %s ", ptr->children.sval);
      gen_code_expr_s(ptr->children.center);
   }
}

/*
void gen_code_expr(tree *ptr){
   if(ptr==NULL) return;
   gen_code_expr_s(ptr->children.left);
   
   if(ptr->children.center != NULL){
      int type = 2;
      struct tree *t = ptr->children.left->children.left->children.left;
      if(t->nodetype == 18){
         struct my_struct *iden;
	 iden = find_sym(t->sval);
	 if(iden == NULL) printf("Error, undeclared variable: %s in expression",t->sval);
	 else {
	    if(iden->type != 0){
               printf("Error, type mismatch in expression!");
	    }
	    else{
               printf(" %s ", ptr->children.sval);
               gen_code_expr_s(ptr->children.left);
	    }
	 }
      }
      else if(t->nodetype != 16){
         printf("Error, type mismatch in expression!");
      }
      else{
         printf(" %s ", ptr->children.sval);
         gen_code_expr_s(ptr->children.left);
      }
   }
}
*/
void gen_code_asgn_rd(tree *ptr){
   char*varname = ptr->children.sval;
   struct my_struct *s;
   s = find_sym(varname);
   if(s == NULL){
      printf("Error, undeclared variable: %s\n", varname);
   }
   else{
      if(s->type != 0) printf("Error, type mismatch in readInt!\n");
      else{
         printf("%s = readInt()", ptr->children.sval);
         printf(";\n");
	 }
   }
}

void gen_code_asgn(tree *ptr){
   char*varname = ptr->children.sval;
   struct my_struct *s;
   s = find_sym(varname);
   if(s == NULL){
      printf("Error, undeclared variable: %s in assignment\n", varname);
   }
   else {
      int type = 2;
      struct tree *t = ptr->children.left->children.left->children.left->children.left;
      if(t->nodetype == 18){
         struct my_struct *iden;
	 iden = find_sym(t->sval);
	 if(iden == NULL) printf("Error, undeclared variable: %s in assignment\n",t->sval);
	 else {
	    if(s->type != iden->type){
               printf("Error, type mismatch in assignment!\n");
	    }
	    else{
               printf("%s = ", varname);
               gen_code_expr(ptr->children.left);
               printf(";\n");
	    }
	 }
      }
      else if(t->nodetype == 16){
         if(s->type == 1) printf("Error, type mismatch in assignment!\n");
	 else{
            printf("%s = ", varname);
            gen_code_expr(ptr->children.left);
            printf(";\n");
	 }
      }
      else if(t->nodetype == 17){
         if(s->type == 0) printf("Error, type mismatch in assignment!\n");
	 else{
            printf("%s = ", varname);
            gen_code_expr(ptr->children.left);
            printf(";\n");
	 }
      }
      }
}

void gen_code_else(tree *ptr, int tcount){
   if(ptr==NULL) return;
   else{
      printf("else{\n ");
      gen_code_stmt_seq(ptr->children.left, ++tcount);
      print_tabs(--tcount);
      printf("}\n");
   }
}

void gen_code_if(tree *ptr, int tcount){
   printf("if( ");
   gen_code_expr(ptr->children.left);
   printf(" ){\n");
   gen_code_stmt_seq(ptr->children.center, ++tcount);
   print_tabs(--tcount);
   printf("}\n");
   gen_code_else(ptr->children.right, tcount);

}

void gen_code_while(tree *ptr, int tcount){
   printf("while( ");
   gen_code_expr(ptr->children.left);
   printf(" ){\n");
   gen_code_stmt_seq(ptr->children.center,++tcount);
   print_tabs(--tcount);
   printf("}\n");
}

void gen_code_write_int(tree *ptr, int tcount){
   printf("writeInt(");
   struct tree *t = ptr->children.left->children.left->children.left->children.left;
   if(t->nodetype == 18){
         struct my_struct *iden;
	 iden = find_sym(t->sval);
	 if(iden == NULL) printf("Error, undeclared variable: %s in writeInt);\n",t->sval);
	 else {
	    if(iden->type==1){
               printf("Error, writeInt takes an integer!);\n");
	    }
	    else {
               gen_code_expr(ptr->children.left);
               printf(");\n");
	    }
	 }
   }

   else if(t->nodetype == 16){
      gen_code_expr(ptr->children.left);
      printf(");\n");
   }
}

void gen_code_stmt(tree *ptr, int tcount){
   ptr = ptr->children.left;
   if(ptr==NULL) return;
   else{
      if(ptr->nodetype == asgn_node) gen_code_asgn(ptr);
      else if(ptr->nodetype == asgn_rd_node) gen_code_asgn_rd(ptr);
      else if(ptr->nodetype == if_node) gen_code_if(ptr, tcount);
      else if(ptr->nodetype == else_node) gen_code_else(ptr, tcount);
      else if(ptr->nodetype == while_node) gen_code_while(ptr, tcount);
      else if(ptr->nodetype == write_int_node) gen_code_write_int(ptr,tcount);
   }
}

void gen_code_stmt_seq(tree *ptr, int tcount){
   if(ptr==NULL) return;
   else{
      print_tabs(tcount);
      gen_code_stmt(ptr->children.left, tcount);
      ptr = ptr->children.center;
      gen_code_stmt_seq(ptr, tcount);
   }
}

void gen_code_decl(tree *ptr, int tcount){
   if(ptr==NULL) return;
   else{
      print_tabs(tcount);
      add_sym(count++, ptr->children.sval, ptr->children.val);
//      if(ptr->children.val==0) printf("int %s;\n", ptr->children.sval);
//      else printf("bool %s;\n", ptr->children.sval);
      ptr = ptr->children.left;
      gen_code_decl(ptr, tcount);
   }
}

void gen_code(tree *decl, tree *stmt_seq){
   printf("\n\n");
   printf("#include <stdio.h>\n\n");
   printf("int main(){\n\n");
   gen_code_decl(decl,1);
   gen_code_stmt_seq(stmt_seq,1);
   printf("\n\treturn 0;\n}\n");
}

