#include "bn_s.h"

void bignum_signed_init(struct bn_s* n){
	bignum_init(&n->value);
	n->sign = 0;
}

void bignum_signed_from_bn(struct bn_s* n, struct bn* bigint){
	bignum_assign(&n->value, bigint);
	n->sign = 0;
}

void bignum_signed_from_int(struct bn_s* n, int64_t value){
	int64_t value_abs = value;
	uint8_t sign = 0;
	if(value_abs < 0){
		value_abs = -value_abs;
		sign = 1;
	}
	bignum_from_int(&n->value, value_abs);
	n->sign = sign;
}

void bignum_signed_add(struct bn_s* a, struct bn_s* b, struct bn_s* c){
	if(a->sign == b->sign){
		bignum_add(&a->value, &b->value, &c->value);
		c->sign = a->sign;
	}else if(!a->sign && b->sign){
		int result = bignum_cmp(&a->value, &b->value);
		switch(result){
			case LARGER:{
					    bignum_sub(&a->value, &b->value, &c->value);
					    c->sign = 0;
				    }
				    break;
			case SMALLER:{
					    bignum_sub(&b->value, &a->value, &c->value);
					    c->sign = 1;
				     }
				     break;
			case EQUAL:{
					   bignum_init(&c->value);
					   c->sign = 0;
				   }
				   break;
		}
	}else if(a->sign && !b->sign){
		int result = bignum_cmp(&b->value, &a->value);
		switch(result){
			case LARGER:{
					    bignum_sub(&b->value, &a->value, &c->value);
					    c->sign = 0;
				    }
				    break;
			case SMALLER:{
					    bignum_sub(&a->value, &b->value, &c->value);
					    c->sign = 1;
				     }
				     break;
			case EQUAL:{
					   bignum_init(&c->value);
					   c->sign = 0;
				   }
				   break;
		}
	}	
}

void bignum_signed_sub(struct bn_s* a, struct bn_s* b, struct bn_s* c){
	struct bn_s tmp;
	bignum_assign(&tmp.value, &b->value);
	tmp.sign = 1;
	bignum_signed_add(a, &tmp, c);
}


void bignum_signed_mul(struct bn_s* a, struct bn_s* b, struct bn_s* c){
	if(a->sign == b->sign){
		c->sign = 0;
		bignum_mul(&a->value, &b->value, &c->value);
	}else{
		c->sign = 1;
		bignum_mul(&a->value, &b->value, &c->value);
	}
}


void bignum_signed_div(struct bn_s* a, struct bn_s* b, struct bn_s* c){
	if(a->sign == b->sign){
		c->sign = 0;
		bignum_div(&a->value, &b->value, &c->value);
	}else{
		c->sign = 1;
		bignum_div(&a->value, &b->value, &c->value);
	}
}

int bignum_signed_cmp(struct bn_s* a, struct bn_s* b){
	if(!a->sign && !b->sign){
		return(bignum_cmp(&a->value, &b->value));
	}else if(!a->sign && b->sign){
		return(LARGER);
	}else if(a->sign && !b->sign){
		return(SMALLER);
	}else{//both are negative
		int result = bignum_cmp(&a->value, &b->value);
		switch(result){
			case LARGER:
				return(SMALLER);
			case SMALLER:
				return(LARGER);
			default:
				return(result);
		}
	}
}

void bignum_signed_assign(struct bn_s* dst, struct bn_s* src){
	bignum_assign(&dst->value, &src->value);
	dst->sign = src->sign;
}

int bignum_signed_is_zero(struct bn_s* n){
	return(bignum_is_zero(&n->value));
}

int bignum_signed_is_negative(struct bn_s* n){
	return(n->sign == 1);
}
