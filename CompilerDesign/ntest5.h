#include <stdio.h>

enum treetype {prog_node, decl_node, stmt_seq_node, stmt_node, asgn_node, asgn_rd_node, if_node, else_node, while_node, write_int_node, expr_node, expr2_node, expr_s_node, expr_s2_node, term1_node, term2_node, factor_num_node, factor_boollit_node, factor_ident_node, factor_expr_node};
typedef struct tree{
   enum treetype nodetype;
   struct {
      struct tree *left, *center, *right; char* sval; int val;
   } children;
   char *sval;
   int val;
} tree;


struct my_struct{
   int id;
   char name[20];
   int type;
   UT_hash_handle hh;
};

struct my_struct *sym = NULL;

void add_sym(int sym_id, char*name, int type){
   struct my_struct *s;

   HASH_FIND_STR(sym, name, s);
   if(s==NULL){
      s = (struct my_struct*) malloc(sizeof(struct my_struct));
      strcpy(s->name, name);
      s->id = sym_id;
      HASH_ADD_STR( sym, name, s);
      if(type==0) printf("int ");
      else printf("bool ");
      printf("%s;\n",name);
   }
   else{
      printf("Error, variable already declared!\n");
   }
   s->type = type;
}

struct my_struct *find_sym(char* name){
   struct my_struct *s;

   HASH_FIND_STR(sym, name, s);
   return s;
}

void print_syms(){
   struct my_struct *s;

   for(s=sym; s != NULL; s=(struct my_struct*)(s->hh.next)){
      printf("sym id %d: name %s: type: %d\n", s->id, s->name, s->type);
   }
}

