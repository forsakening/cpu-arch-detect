/*Modify from gcc-5.x for detect intel cpu march @20180105*/
#include <stdio.h>

/* %ecx */
#define bit_SSE3	(1 << 0)
#define bit_PCLMUL	(1 << 1)
#define bit_LZCNT	(1 << 5)
#define bit_SSSE3	(1 << 9)
#define bit_FMA		(1 << 12)
#define bit_CMPXCHG16B	(1 << 13)
#define bit_SSE4_1	(1 << 19)
#define bit_SSE4_2	(1 << 20)
#define bit_MOVBE	(1 << 22)
#define bit_POPCNT	(1 << 23)
#define bit_AES		(1 << 25)
#define bit_XSAVE	(1 << 26)
#define bit_OSXSAVE	(1 << 27)
#define bit_AVX		(1 << 28)
#define bit_F16C	(1 << 29)
#define bit_RDRND	(1 << 30)

/* %edx */
#define bit_CMPXCHG8B	(1 << 8)
#define bit_CMOV	(1 << 15)
#define bit_MMX		(1 << 23)
#define bit_FXSAVE	(1 << 24)
#define bit_SSE		(1 << 25)
#define bit_SSE2	(1 << 26)

/* Extended Features */
/* %ecx */
#define bit_LAHF_LM	(1 << 0)
#define bit_ABM		(1 << 5)
#define bit_SSE4a	(1 << 6)
#define bit_PRFCHW	(1 << 8)
#define bit_XOP         (1 << 11)
#define bit_LWP 	(1 << 15)
#define bit_FMA4        (1 << 16)
#define bit_TBM         (1 << 21)

/* %edx */
#define bit_MMXEXT	(1 << 22)
#define bit_LM		(1 << 29)
#define bit_3DNOWP	(1 << 30)
#define bit_3DNOW	(1 << 31)

/* Extended Features (%eax == 7) */
/* %ebx */
#define bit_FSGSBASE	(1 << 0)
#define bit_BMI	(1 << 3)
#define bit_HLE	(1 << 4)
#define bit_AVX2	(1 << 5)
#define bit_BMI2	(1 << 8)
#define bit_RTM	(1 << 11)
#define bit_MPX	(1 << 14)
#define bit_AVX512F	(1 << 16)
#define bit_AVX512DQ	(1 << 17)
#define bit_RDSEED	(1 << 18)
#define bit_ADX	(1 << 19)
#define bit_AVX512IFMA	(1 << 21)
#define bit_PCOMMIT	(1 << 22)
#define bit_CLFLUSHOPT	(1 << 23)
#define bit_CLWB	(1 << 24)
#define bit_AVX512PF	(1 << 26)
#define bit_AVX512ER	(1 << 27)
#define bit_AVX512CD	(1 << 28)
#define bit_SHA		(1 << 29)
#define bit_AVX512BW	(1 << 30)
#define bit_AVX512VL	(1 << 31)

/* %ecx */
#define bit_PREFETCHWT1	  (1 << 0)
#define bit_AVX512VBMI	(1 << 1)

/* XFEATURE_ENABLED_MASK register bits (%eax == 13, %ecx == 0) */
#define bit_BNDREGS     (1 << 3)
#define bit_BNDCSR      (1 << 4)

/* Extended State Enumeration Sub-leaf (%eax == 13, %ecx == 1) */
#define bit_XSAVEOPT	(1 << 0)
#define bit_XSAVEC	(1 << 1)
#define bit_XSAVES	(1 << 3)

/* Signatures for different CPU implementations as returned in uses
   of cpuid with level 0.  */
#define signature_AMD_ebx	0x68747541
#define signature_AMD_ecx	0x444d4163
#define signature_AMD_edx	0x69746e65

#define signature_CENTAUR_ebx	0x746e6543
#define signature_CENTAUR_ecx	0x736c7561
#define signature_CENTAUR_edx	0x48727561

#define signature_CYRIX_ebx	0x69727943
#define signature_CYRIX_ecx	0x64616574
#define signature_CYRIX_edx	0x736e4978

#define signature_INTEL_ebx	0x756e6547
#define signature_INTEL_ecx	0x6c65746e
#define signature_INTEL_edx	0x49656e69

#define signature_TM1_ebx	0x6e617254
#define signature_TM1_ecx	0x55504361
#define signature_TM1_edx	0x74656d73

#define signature_TM2_ebx	0x756e6547
#define signature_TM2_ecx	0x3638784d
#define signature_TM2_edx	0x54656e69

#define signature_NSC_ebx	0x646f6547
#define signature_NSC_ecx	0x43534e20
#define signature_NSC_edx	0x79622065

#define signature_NEXGEN_ebx	0x4778654e
#define signature_NEXGEN_ecx	0x6e657669
#define signature_NEXGEN_edx	0x72446e65

#define signature_RISE_ebx	0x65736952
#define signature_RISE_ecx	0x65736952
#define signature_RISE_edx	0x65736952

#define signature_SIS_ebx	0x20536953
#define signature_SIS_ecx	0x20536953
#define signature_SIS_edx	0x20536953

#define signature_UMC_ebx	0x20434d55
#define signature_UMC_ecx	0x20434d55
#define signature_UMC_edx	0x20434d55

#define signature_VIA_ebx	0x20414956
#define signature_VIA_ecx	0x20414956
#define signature_VIA_edx	0x20414956

#define signature_VORTEX_ebx	0x74726f56
#define signature_VORTEX_ecx	0x436f5320
#define signature_VORTEX_edx	0x36387865

enum processor_type
{
  PROCESSOR_GENERIC = 0,
  PROCESSOR_I386,			/* 80386 */
  PROCESSOR_I486,			/* 80486DX, 80486SX, 80486DX[24] */
  PROCESSOR_PENTIUM,
  PROCESSOR_PENTIUMPRO,
  PROCESSOR_PENTIUM4,
  PROCESSOR_NOCONA,
  PROCESSOR_CORE2,
  PROCESSOR_NEHALEM,
  PROCESSOR_SANDYBRIDGE,
  PROCESSOR_HASWELL,
  PROCESSOR_BONNELL,
  PROCESSOR_SILVERMONT,
  PROCESSOR_KNL,
  PROCESSOR_INTEL,
  PROCESSOR_GEODE,
  PROCESSOR_K6,
  PROCESSOR_ATHLON,
  PROCESSOR_K8,
  PROCESSOR_AMDFAM10,
  PROCESSOR_BDVER1,
  PROCESSOR_BDVER2,
  PROCESSOR_BDVER3,
  PROCESSOR_BDVER4,
  PROCESSOR_BTVER1,
  PROCESSOR_BTVER2,
  PROCESSOR_max
};

#define __cpuid(level, a, b, c, d)			\
  __asm__ ("cpuid\n\t"					\
	   : "=a" (a), "=b" (b), "=c" (c), "=d" (d)	\
	   : "0" (level))

#define __cpuid_count(level, count, a, b, c, d)		\
  __asm__ ("cpuid\n\t"					\
	   : "=a" (a), "=b" (b), "=c" (c), "=d" (d)	\
	   : "0" (level), "2" (count))


/* Return highest supported input value for cpuid instruction.  ext can
   be either 0x0 or 0x8000000 to return highest supported value for
   basic or extended cpuid information.  Function returns 0 if cpuid
   is not supported or whatever cpuid returns in eax register.  If sig
   pointer is non-null, then first four bytes of the signature
   (as found in ebx register) are returned in location pointed by sig.  */

static __inline unsigned int
__get_cpuid_max (unsigned int __ext, unsigned int *__sig)
{
  unsigned int __eax, __ebx, __ecx, __edx;

#ifndef __x86_64__
  /* See if we can use cpuid.  On AMD64 we always can.  */
#if __GNUC__ >= 3
  __asm__ ("pushf{l|d}\n\t"
	   "pushf{l|d}\n\t"
	   "pop{l}\t%0\n\t"
	   "mov{l}\t{%0, %1|%1, %0}\n\t"
	   "xor{l}\t{%2, %0|%0, %2}\n\t"
	   "push{l}\t%0\n\t"
	   "popf{l|d}\n\t"
	   "pushf{l|d}\n\t"
	   "pop{l}\t%0\n\t"
	   "popf{l|d}\n\t"
	   : "=&r" (__eax), "=&r" (__ebx)
	   : "i" (0x00200000));
#else
/* Host GCCs older than 3.0 weren't supporting Intel asm syntax
   nor alternatives in i386 code.  */
  __asm__ ("pushfl\n\t"
	   "pushfl\n\t"
	   "popl\t%0\n\t"
	   "movl\t%0, %1\n\t"
	   "xorl\t%2, %0\n\t"
	   "pushl\t%0\n\t"
	   "popfl\n\t"
	   "pushfl\n\t"
	   "popl\t%0\n\t"
	   "popfl\n\t"
	   : "=&r" (__eax), "=&r" (__ebx)
	   : "i" (0x00200000));
#endif

  if (!((__eax ^ __ebx) & 0x00200000))
    return 0;
#endif

  /* Host supports cpuid.  Return highest supported cpuid input value.  */
  __cpuid (__ext, __eax, __ebx, __ecx, __edx);

  if (__sig)
    *__sig = __ebx;

  return __eax;
}

/* Return cpuid data for requested cpuid level, as found in returned
   eax, ebx, ecx and edx registers.  The function checks if cpuid is
   supported and returns 1 for valid cpuid information or 0 for
   unsupported cpuid level.  All pointers are required to be non-null.  */

static __inline int
__get_cpuid (unsigned int __level,
	     unsigned int *__eax, unsigned int *__ebx,
	     unsigned int *__ecx, unsigned int *__edx)
{
  unsigned int __ext = __level & 0x80000000;

  if (__get_cpuid_max (__ext, 0) < __level)
    return 0;

  __cpuid (__level, *__eax, *__ebx, *__ecx, *__edx);
  return 1;
}


int main(int argc, char* argv[])
{
  enum processor_type processor = PROCESSOR_I386;
  const char *cpu = "unkown";
  const char *cache = "";
  const char *options = "";

  unsigned int eax, ebx, ecx, edx;

  unsigned int max_level, ext_level;

  unsigned int vendor;
  unsigned int model, family;

  unsigned int has_sse3, has_ssse3, has_cmpxchg16b;
  unsigned int has_cmpxchg8b, has_cmov, has_mmx, has_sse, has_sse2;

  /* Extended features */
  unsigned int has_lahf_lm = 0, has_sse4a = 0;
  unsigned int has_longmode = 0, has_3dnowp = 0, has_3dnow = 0;
  unsigned int has_movbe = 0, has_sse4_1 = 0, has_sse4_2 = 0;
  unsigned int has_popcnt = 0, has_aes = 0, has_avx = 0, has_avx2 = 0;
  unsigned int has_pclmul = 0, has_abm = 0, has_lwp = 0;
  unsigned int has_fma = 0, has_fma4 = 0, has_xop = 0;
  unsigned int has_bmi = 0, has_bmi2 = 0, has_tbm = 0, has_lzcnt = 0;
  unsigned int has_hle = 0, has_rtm = 0;
  unsigned int has_rdrnd = 0, has_f16c = 0, has_fsgsbase = 0;
  unsigned int has_rdseed = 0, has_prfchw = 0, has_adx = 0;
  unsigned int has_osxsave = 0, has_fxsr = 0, has_xsave = 0, has_xsaveopt = 0;
  unsigned int has_avx512er = 0, has_avx512pf = 0, has_avx512cd = 0;
  unsigned int has_avx512f = 0, has_sha = 0, has_prefetchwt1 = 0;
  unsigned int has_clflushopt = 0, has_xsavec = 0, has_xsaves = 0;
  unsigned int has_avx512dq = 0, has_avx512bw = 0, has_avx512vl = 0;
  unsigned int has_avx512vbmi = 0, has_avx512ifma = 0, has_clwb = 0;
  unsigned int has_pcommit = 0;

  unsigned char arch = 1;
	
  max_level = __get_cpuid_max (0, &vendor);
  if (max_level < 1)
    goto done;

  __cpuid (1, eax, ebx, ecx, edx);

  model = (eax >> 4) & 0x0f;
  family = (eax >> 8) & 0x0f;
  if (vendor == signature_INTEL_ebx
      || vendor == signature_AMD_ebx)
    {
      unsigned int extended_model, extended_family;

      extended_model = (eax >> 12) & 0xf0;
      extended_family = (eax >> 20) & 0xff;
      if (family == 0x0f)
	{
	  family += extended_family;
	  model += extended_model;
	}
      else if (family == 0x06)
	model += extended_model;
    }
  
  has_sse3 = ecx & bit_SSE3;
  has_ssse3 = ecx & bit_SSSE3;
  has_sse4_1 = ecx & bit_SSE4_1;
  has_sse4_2 = ecx & bit_SSE4_2;
  has_avx = ecx & bit_AVX;
  has_osxsave = ecx & bit_OSXSAVE;
  has_cmpxchg16b = ecx & bit_CMPXCHG16B;
  has_movbe = ecx & bit_MOVBE;
  has_popcnt = ecx & bit_POPCNT;
  has_aes = ecx & bit_AES;
  has_pclmul = ecx & bit_PCLMUL;
  has_fma = ecx & bit_FMA;
  has_f16c = ecx & bit_F16C;
  has_rdrnd = ecx & bit_RDRND;
  has_xsave = ecx & bit_XSAVE;

  has_cmpxchg8b = edx & bit_CMPXCHG8B;
  has_cmov = edx & bit_CMOV;
  has_mmx = edx & bit_MMX;
  has_fxsr = edx & bit_FXSAVE;
  has_sse = edx & bit_SSE;
  has_sse2 = edx & bit_SSE2;

  if (max_level >= 7)
    {
      __cpuid_count (7, 0, eax, ebx, ecx, edx);

      has_bmi = ebx & bit_BMI;
      has_hle = ebx & bit_HLE;
      has_rtm = ebx & bit_RTM;
      has_avx2 = ebx & bit_AVX2;
      has_bmi2 = ebx & bit_BMI2;
      has_fsgsbase = ebx & bit_FSGSBASE;
      has_rdseed = ebx & bit_RDSEED;
      has_adx = ebx & bit_ADX;
      has_avx512f = ebx & bit_AVX512F;
      has_avx512er = ebx & bit_AVX512ER;
      has_avx512pf = ebx & bit_AVX512PF;
      has_avx512cd = ebx & bit_AVX512CD;
      has_sha = ebx & bit_SHA;
      has_pcommit = ebx & bit_PCOMMIT;
      has_clflushopt = ebx & bit_CLFLUSHOPT;
      has_clwb = ebx & bit_CLWB;
      has_avx512dq = ebx & bit_AVX512DQ;
      has_avx512bw = ebx & bit_AVX512BW;
      has_avx512vl = ebx & bit_AVX512VL;
      has_avx512vl = ebx & bit_AVX512IFMA;

      has_prefetchwt1 = ecx & bit_PREFETCHWT1;
      has_avx512vl = ecx & bit_AVX512VBMI;
    }

  if (max_level >= 13)
    {
      __cpuid_count (13, 1, eax, ebx, ecx, edx);

      has_xsaveopt = eax & bit_XSAVEOPT;
      has_xsavec = eax & bit_XSAVEC;
      has_xsaves = eax & bit_XSAVES;
    }

  /* Check cpuid level of extended features.  */
  __cpuid (0x80000000, ext_level, ebx, ecx, edx);

  if (ext_level > 0x80000000)
    {
      __cpuid (0x80000001, eax, ebx, ecx, edx);

      has_lahf_lm = ecx & bit_LAHF_LM;
      has_sse4a = ecx & bit_SSE4a;
      has_abm = ecx & bit_ABM;
      has_lwp = ecx & bit_LWP;
      has_fma4 = ecx & bit_FMA4;
      has_xop = ecx & bit_XOP;
      has_tbm = ecx & bit_TBM;
      has_lzcnt = ecx & bit_LZCNT;
      has_prfchw = ecx & bit_PRFCHW;

      has_longmode = edx & bit_LM;
      has_3dnowp = edx & bit_3DNOWP;
      has_3dnow = edx & bit_3DNOW;
    }  

	/* Get XCR_XFEATURE_ENABLED_MASK register with xgetbv.  */
#define XCR_XFEATURE_ENABLED_MASK	0x0
#define XSTATE_FP			0x1
#define XSTATE_SSE			0x2
#define XSTATE_YMM			0x4
#define XSTATE_OPMASK			0x20
#define XSTATE_ZMM			0x40
#define XSTATE_HI_ZMM			0x80
  if (has_osxsave)
    asm (".byte 0x0f; .byte 0x01; .byte 0xd0"
	 : "=a" (eax), "=d" (edx)
	 : "c" (XCR_XFEATURE_ENABLED_MASK));

  /* Check if SSE and YMM states are supported.  */
  if (!has_osxsave
      || (eax & (XSTATE_SSE | XSTATE_YMM)) != (XSTATE_SSE | XSTATE_YMM))
    {
      has_avx = 0;
      has_avx2 = 0;
      has_fma = 0;
      has_fma4 = 0;
      has_f16c = 0;
      has_xop = 0;
      has_xsave = 0;
      has_xsaveopt = 0;
      has_xsaves = 0;
      has_xsavec = 0;
    }

  if (!has_osxsave
      || (eax &
	  (XSTATE_SSE | XSTATE_YMM | XSTATE_OPMASK | XSTATE_ZMM | XSTATE_HI_ZMM))
	  != (XSTATE_SSE | XSTATE_YMM | XSTATE_OPMASK | XSTATE_ZMM | XSTATE_HI_ZMM))
    {
      has_avx512f = 0;
      has_avx512er = 0;
      has_avx512pf = 0;
      has_avx512cd = 0;
      has_avx512dq = 0;
      has_avx512bw = 0;
      has_avx512vl = 0;
    }
	
  if (vendor == signature_AMD_ebx)
    {
      unsigned int name;

      /* Detect geode processor by its processor signature.  */
      if (ext_level > 0x80000001)
	__cpuid (0x80000002, name, ebx, ecx, edx);
      else
	name = 0;

      if (name == signature_NSC_ebx)
	processor = PROCESSOR_GEODE;
      else if (has_movbe && family == 22)
	processor = PROCESSOR_BTVER2;
      else if (has_avx2)
        processor = PROCESSOR_BDVER4;
      else if (has_xsaveopt)
        processor = PROCESSOR_BDVER3;
      else if (has_bmi)
        processor = PROCESSOR_BDVER2;
      else if (has_xop)
	processor = PROCESSOR_BDVER1;
      else if (has_sse4a && has_ssse3)
        processor = PROCESSOR_BTVER1;
      else if (has_sse4a)
	processor = PROCESSOR_AMDFAM10;
      else if (has_sse2 || has_longmode)
	processor = PROCESSOR_K8;
      else if (has_3dnowp && family == 6)
	processor = PROCESSOR_ATHLON;
      else if (has_mmx)
	processor = PROCESSOR_K6;
      else
	processor = PROCESSOR_PENTIUM;
    }
  else if (vendor == signature_CENTAUR_ebx)
    {
      if (arch)
	{
	  switch (family)
	    {
	    case 6:
	      if (model > 9)
		/* Use the default detection procedure.  */
		processor = PROCESSOR_GENERIC;
	      else if (model == 9)
		cpu = "c3-2";
	      else if (model >= 6)
		cpu = "c3";
	      else
		processor = PROCESSOR_GENERIC;
	      break;
	    case 5:
	      if (has_3dnow)
		cpu = "winchip2";
	      else if (has_mmx)
		cpu = "winchip2-c6";
	      else
		processor = PROCESSOR_GENERIC;
	      break;
	    default:
	      /* We have no idea.  */
	      processor = PROCESSOR_GENERIC;
	    }
	}
    }
  else
    {
      switch (family)
	{
	case 4:
	  processor = PROCESSOR_I486;
	  break;
	case 5:
	  processor = PROCESSOR_PENTIUM;
	  break;
	case 6:
	  processor = PROCESSOR_PENTIUMPRO;
	  break;
	case 15:
	  processor = PROCESSOR_PENTIUM4;
	  break;
	default:
	  /* We have no idea.  */
	  processor = PROCESSOR_GENERIC;
	}
    }

  switch (processor)
    {
    case PROCESSOR_I386:
      /* Default.  */
      break;
    case PROCESSOR_I486:
      cpu = "i486";
      break;
    case PROCESSOR_PENTIUM:
      if (arch && has_mmx)
	cpu = "pentium-mmx";
      else
	cpu = "pentium";
      break;
    case PROCESSOR_PENTIUMPRO:
      switch (model)
	{
	case 0x1c:
	case 0x26:
	  /* Bonnell.  */
	  cpu = "bonnell";
	  break;
	case 0x37:
	case 0x4a:
	case 0x4d:
	case 0x5a:
	case 0x5d:
	  /* Silvermont.  */
	  cpu = "silvermont";
	  break;
	case 0x0f:
	  /* Merom.  */
	case 0x17:
	case 0x1d:
	  /* Penryn.  */
	  cpu = "core2";
	  break;
	case 0x1a:
	case 0x1e:
	case 0x1f:
	case 0x2e:
	  /* Nehalem.  */
	  cpu = "nehalem";
	  break;
	case 0x25:
	case 0x2c:
	case 0x2f:
	  /* Westmere.  */
	  cpu = "westmere";
	  break;
	case 0x2a:
	case 0x2d:
	  /* Sandy Bridge.  */
	  cpu = "sandybridge";
	  break;
	case 0x3a:
	case 0x3e:
	  /* Ivy Bridge.  */
	  cpu = "ivybridge";
	  break;
	case 0x3c:
	case 0x3f:
	case 0x45:
	case 0x46:
	  /* Haswell.  */
	  cpu = "haswell";
	  break;
	case 0x3d:
	case 0x4f:
	case 0x56:
	  /* Broadwell.  */
	  cpu = "broadwell";
	  break;
	case 0x57:
	  /* Knights Landing.  */
	  cpu = "knl";
	  break;
	default:
	  if (arch)
	    {
	      /* This is unknown family 0x6 CPU.  */
	      /* Assume Knights Landing.  */
	      if (has_avx512f)
		cpu = "knl";
	      /* Assume Broadwell.  */
	      else if (has_adx)
		cpu = "broadwell";
	      else if (has_avx2)
		/* Assume Haswell.  */
		cpu = "haswell";
	      else if (has_avx)
		/* Assume Sandy Bridge.  */
		cpu = "sandybridge";
	      else if (has_sse4_2)
		{
		  if (has_movbe)
		    /* Assume Silvermont.  */
		    cpu = "silvermont";
		  else
		    /* Assume Nehalem.  */
		    cpu = "nehalem";
		}
	      else if (has_ssse3)
		{
		  if (has_movbe)
		    /* Assume Bonnell.  */
		    cpu = "bonnell";
		  else
		    /* Assume Core 2.  */
		    cpu = "core2";
		}
	      else if (has_longmode)
		/* Perhaps some emulator?  Assume x86-64, otherwise gcc
		   -march=native would be unusable for 64-bit compilations,
		   as all the CPUs below are 32-bit only.  */
		cpu = "x86-64";
	      else if (has_sse3)
		/* It is Core Duo.  */
		cpu = "pentium-m";
	      else if (has_sse2)
		/* It is Pentium M.  */
		cpu = "pentium-m";
	      else if (has_sse)
		/* It is Pentium III.  */
		cpu = "pentium3";
	      else if (has_mmx)
		/* It is Pentium II.  */
		cpu = "pentium2";
	      else
		/* Default to Pentium Pro.  */
		cpu = "pentiumpro";
	    }
	  else
	    /* For -mtune, we default to -mtune=generic.  */
	    cpu = "generic";
	  break;
	}
      break;
    case PROCESSOR_PENTIUM4:
      if (has_sse3)
	{
	  if (has_longmode)
	    cpu = "nocona";
	  else
	    cpu = "prescott";
	}
      else
	cpu = "pentium4";
      break;
    case PROCESSOR_GEODE:
      cpu = "geode";
      break;
    case PROCESSOR_K6:
      if (arch && has_3dnow)
	cpu = "k6-3";
      else
	cpu = "k6";
      break;
    case PROCESSOR_ATHLON:
      if (arch && has_sse)
	cpu = "athlon-4";
      else
	cpu = "athlon";
      break;
    case PROCESSOR_K8:
      if (arch && has_sse3)
	cpu = "k8-sse3";
      else
	cpu = "k8";
      break;
    case PROCESSOR_AMDFAM10:
      cpu = "amdfam10";
      break;
    case PROCESSOR_BDVER1:
      cpu = "bdver1";
      break;
    case PROCESSOR_BDVER2:
      cpu = "bdver2";
      break;
    case PROCESSOR_BDVER3:
      cpu = "bdver3";
      break;
    case PROCESSOR_BDVER4:
      cpu = "bdver4";
      break;
    case PROCESSOR_BTVER1:
      cpu = "btver1";
      break;
    case PROCESSOR_BTVER2:
      cpu = "btver2";
      break;

    default:
      /* Use something reasonable.  */
      if (arch)
	{
	  if (has_ssse3)
	    cpu = "core2";
	  else if (has_sse3)
	    {
	      if (has_longmode)
		cpu = "nocona";
	      else
		cpu = "prescott";
	    }
	  else if (has_sse2)
	    cpu = "pentium4";
	  else if (has_cmov)
	    cpu = "pentiumpro";
	  else if (has_mmx)
	    cpu = "pentium-mmx";
	  else if (has_cmpxchg8b)
	    cpu = "pentium";
	}
      else
	cpu = "generic";
    }

done:
	printf("%s\n", cpu);
    return 0;
}