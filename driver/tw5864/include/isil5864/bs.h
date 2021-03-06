#ifndef _BS_H
#define _BS_H

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct bs_s
    {
	__u8	*p_start;
	__u8	*p;
	__u8	*p_end;

	int		i_left;    /* i_count number of available bits */
    }bs_t;

    static inline void bs_init(bs_t *s, void *p_data, int i_data)
    {
	s->p_start = p_data;
	s->p       = p_data;
	s->p_end   = s->p + i_data;
	s->i_left  = 8;
    }

    static inline int	bs_pos( bs_t *s )
    {
	return( 8 * ( s->p - s->p_start ) + 8 - s->i_left );
    }

    static inline int	bs_eof( bs_t *s )
    {
	return( s->p >= s->p_end ? 1: 0 );
    }

    static inline int	bs_len(bs_t *s)
    {
	return (s->p - s->p_start);
    }

    static inline int	bs_left(bs_t *s)
    {
	return (8-s->i_left);
    }

    static inline void	bs_direct_write(bs_t *s, u8 value)
    {
	*s->p = value;
	s->p++;
	s->i_left = 8;
    }

    static inline void	bs_write(bs_t *s, int i_count, u32 i_bits)
    {
	if( s->p >= s->p_end - 4 )
	    return;
	while( i_count > 0 )
	{
	    if( i_count < 32 )
		i_bits &= (1<<i_count)-1;
	    if( i_count < s->i_left )
	    {
		*s->p = (*s->p << i_count) | i_bits;
		s->i_left -= i_count;
		break;
	    }
	    else
	    {
		*s->p = (*s->p << s->i_left) | (i_bits >> (i_count - s->i_left));
		i_count -= s->i_left;
		s->p++;
		s->i_left = 8;
	    }
	}
    }

    static inline void	bs_write1( bs_t *s, u32 i_bit )
    {
	if( s->p < s->p_end )
	{
	    *s->p <<= 1;
	    *s->p |= i_bit;
	    s->i_left--;
	    if( s->i_left == 0 )
	    {
		s->p++;
		s->i_left = 8;
	    }
	}
    }

    static inline void	bs_align_0( bs_t *s )
    {
	if( s->i_left != 8 )
	{
	    *s->p <<= s->i_left;
	    s->i_left = 8;
	    s->p++;
	}
    }

    static inline void	bs_sh_align( bs_t *s )
    {
	if( s->i_left != 8 )
	{
	    *s->p <<= s->i_left;
	    s->i_left = 8;
	}
    }

    static inline void	bs_align_1( bs_t *s )
    {
	if( s->i_left != 8 )
	{
	    *s->p <<= s->i_left;
	    *s->p |= (1 << s->i_left) - 1;
	    s->i_left = 8;
	    s->p++;
	}
    }

    static inline void	bs_align( bs_t *s )
    {
	bs_align_0( s );
    }

    /* golomb functions */
    static inline void	bs_write_ue( bs_t *s, u32 val )
    {
	int i_size = 0;
	static const int i_size0_255[256] =
	{
	    1,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
	};

	if( val == 0 )
	{
	    bs_write1( s, 1 );
	}
	else
	{
	    unsigned int tmp = ++val;

	    if( tmp >= 0x00010000 )
	    {
		i_size += 16;
		tmp >>= 16;
	    }
	    if( tmp >= 0x100 )
	    {
		i_size += 8;
		tmp >>= 8;
	    }
	    i_size += i_size0_255[tmp];

	    bs_write( s, 2 * i_size - 1, val );
	}
    }

    static inline void	bs_write_se( bs_t *s, int val )
    {
	bs_write_ue( s, val <= 0 ? -val * 2 : val * 2 - 1);
    }

    static inline void	bs_write_te( bs_t *s, int x, int val )
    {
	if( x == 1 )
	{
	    bs_write1( s, 1&~val );
	}
	else if( x > 1 )
	{
	    bs_write_ue( s, val );
	}
    }

    static inline void	bs_rbsp_trailing( bs_t *s )
    {
	bs_write1( s, 1 );
	if( s->i_left != 8 )
	{
	    bs_write( s, s->i_left, 0x00 );
	}
    }

    static inline int	bs_size_ue( unsigned int val )
    {
	static const int i_size0_254[255] =
	{
	    1, 3, 3, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 7,
	    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
	    11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
	    11,11,11,11,11,11,11,11,11,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
	    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
	    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
	    13,13,13,13,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
	    15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
	    15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
	    15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
	    15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
	    15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15
	};

	if( val < 255 )
	{
	    return i_size0_254[val];
	}
	else
	{
	    int i_size = 0;

	    val++;

	    if( val >= 0x10000 )
	    {
		i_size += 32;
		val = (val >> 16) - 1;
	    }
	    if( val >= 0x100 )
	    {
		i_size += 16;
		val = (val >> 8) - 1;
	    }
	    return i_size0_254[val] + i_size;
	}
    }

    static inline int	bs_size_se( int val )
    {
	return bs_size_ue( val <= 0 ? -val * 2 : val * 2 - 1);
    }

    static inline int	bs_size_te( int x, int val )
    {
	if( x == 1 )
	{
	    return 1;
	}
	else if( x > 1 )
	{
	    return bs_size_ue( val );
	}
	return 0;
    }

#ifdef __cplusplus
}
#endif


#endif

