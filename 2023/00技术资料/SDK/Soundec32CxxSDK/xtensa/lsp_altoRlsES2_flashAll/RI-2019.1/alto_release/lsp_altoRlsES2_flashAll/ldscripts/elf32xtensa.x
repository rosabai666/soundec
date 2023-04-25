/* This linker script generated from xt-genldscripts.tpp for LSP D:/SW/SourceCode/Soundec32CxxSDK20/V2.2/Soundec32CxxSDK/xtensa/lsp_altoRlsES2_flashAll/RI-2019.1/alto_release/lsp_altoRlsES2_flashAll */
/* Linker Script for default link */
MEMORY
{
  srom0_seg :                         	org = 0x90000000, len = 0x10000
  dram0_0_seg :                       	org = 0x9FF40000, len = 0x40000
  iram0_0_seg :                       	org = 0x9FFC0000, len = 0x17C
  iram0_1_seg :                       	org = 0x9FFC017C, len = 0x24
  iram0_2_seg :                       	org = 0x9FFC01A0, len = 0x40
  iram0_3_seg :                       	org = 0x9FFC01E0, len = 0x40
  iram0_4_seg :                       	org = 0x9FFC0220, len = 0x3C
  iram0_5_seg :                       	org = 0x9FFC025C, len = 0x20
  iram0_6_seg :                       	org = 0x9FFC027C, len = 0x20
  iram0_7_seg :                       	org = 0x9FFC029C, len = 0x3FD64
  sram0_seg :                         	org = 0xA0000000, len = 0x1000
  sram1_seg :                         	org = 0xA0001000, len = 0xFF000
}

PHDRS
{
  srom0_phdr PT_LOAD;
  dram0_0_phdr PT_LOAD;
  dram0_0_bss_phdr PT_LOAD;
  iram0_0_phdr PT_LOAD;
  iram0_1_phdr PT_LOAD;
  iram0_2_phdr PT_LOAD;
  iram0_3_phdr PT_LOAD;
  iram0_4_phdr PT_LOAD;
  iram0_5_phdr PT_LOAD;
  iram0_6_phdr PT_LOAD;
  iram0_7_phdr PT_LOAD;
  sram0_phdr PT_LOAD;
  sram1_phdr PT_LOAD;
}


/*  Default entry point:  */
ENTRY(_ResetVector)


/*  Memory boundary addresses:  */
_memmap_mem_srom_start = 0x90000000;
_memmap_mem_srom_end   = 0x90010000;
_memmap_mem_dram0_start = 0x9ff40000;
_memmap_mem_dram0_end   = 0x9ff80000;
_memmap_mem_iram0_start = 0x9ffc0000;
_memmap_mem_iram0_end   = 0xa0000000;
_memmap_mem_sram_start = 0xa0000000;
_memmap_mem_sram_end   = 0xa0100000;

/*  Memory segment boundary addresses:  */
_memmap_seg_srom0_start = 0x90000000;
_memmap_seg_srom0_max   = 0x90010000;
_memmap_seg_dram0_0_start = 0x9ff40000;
_memmap_seg_dram0_0_max   = 0x9ff80000;
_memmap_seg_iram0_0_start = 0x9ffc0000;
_memmap_seg_iram0_0_max   = 0x9ffc017c;
_memmap_seg_iram0_1_start = 0x9ffc017c;
_memmap_seg_iram0_1_max   = 0x9ffc01a0;
_memmap_seg_iram0_2_start = 0x9ffc01a0;
_memmap_seg_iram0_2_max   = 0x9ffc01e0;
_memmap_seg_iram0_3_start = 0x9ffc01e0;
_memmap_seg_iram0_3_max   = 0x9ffc0220;
_memmap_seg_iram0_4_start = 0x9ffc0220;
_memmap_seg_iram0_4_max   = 0x9ffc025c;
_memmap_seg_iram0_5_start = 0x9ffc025c;
_memmap_seg_iram0_5_max   = 0x9ffc027c;
_memmap_seg_iram0_6_start = 0x9ffc027c;
_memmap_seg_iram0_6_max   = 0x9ffc029c;
_memmap_seg_iram0_7_start = 0x9ffc029c;
_memmap_seg_iram0_7_max   = 0xa0000000;
_memmap_seg_sram0_start = 0xa0000000;
_memmap_seg_sram0_max   = 0xa0001000;
_memmap_seg_sram1_start = 0xa0001000;
_memmap_seg_sram1_max   = 0xa0100000;

PROVIDE(_memmap_reset_vector = 0xa0000000);
PROVIDE(_memmap_vecbase_reset = 0x9ffc0000);
/* Various memory-map dependent cache attribute settings: */
_memmap_cacheattr_wb_base = 0x00440000;
_memmap_cacheattr_wt_base = 0x00110000;
_memmap_cacheattr_bp_base = 0x00220000;
_memmap_cacheattr_unused_mask = 0xFF00FFFF;
_memmap_cacheattr_wb_trapnull = 0x2244222F;
_memmap_cacheattr_wba_trapnull = 0x2244222F;
_memmap_cacheattr_wbna_trapnull = 0x2255222F;
_memmap_cacheattr_wt_trapnull = 0x2211222F;
_memmap_cacheattr_bp_trapnull = 0x2222222F;
_memmap_cacheattr_wb_strict = 0xFF44FFFF;
_memmap_cacheattr_wt_strict = 0xFF11FFFF;
_memmap_cacheattr_bp_strict = 0xFF22FFFF;
_memmap_cacheattr_wb_allvalid = 0x22442222;
_memmap_cacheattr_wt_allvalid = 0x22112222;
_memmap_cacheattr_bp_allvalid = 0x22222222;
_memmap_region_map = 0x00000030;
PROVIDE(_memmap_cacheattr_reset = _memmap_cacheattr_wb_trapnull);

SECTIONS
{

  .ResetHandler.text : ALIGN(4)
  {
    _ResetHandler_text_start = ABSOLUTE(.);
    KEEP (*(.ResetHandler.text))
    . = ALIGN(4);
    _rom_store_table = .;
    LONG(_sram_rodata_start);
    LONG(_sram_rodata_end);
    LONG(LOADADDR(.sram.rodata));
    LONG(_sram_data_start);
    LONG(_sram_data_end);
    LONG(LOADADDR(.sram.data));
    LONG(__llvm_prf_names_start);
    LONG(__llvm_prf_names_end);
    LONG(LOADADDR(__llvm_prf_names));
    LONG(_dram0_rodata_start);
    LONG(_dram0_rodata_end);
    LONG(LOADADDR(.dram0.rodata));
    LONG(_clib_rodata_start);
    LONG(_clib_rodata_end);
    LONG(LOADADDR(.clib.rodata));
    LONG(_rtos_rodata_start);
    LONG(_rtos_rodata_end);
    LONG(LOADADDR(.rtos.rodata));
    LONG(_rodata_start);
    LONG(_rodata_end);
    LONG(LOADADDR(.rodata));
    LONG(_clib_data_start);
    LONG(_clib_data_end);
    LONG(LOADADDR(.clib.data));
    LONG(_clib_percpu_data_start);
    LONG(_clib_percpu_data_end);
    LONG(LOADADDR(.clib.percpu.data));
    LONG(_rtos_percpu_data_start);
    LONG(_rtos_percpu_data_end);
    LONG(LOADADDR(.rtos.percpu.data));
    LONG(_rtos_data_start);
    LONG(_rtos_data_end);
    LONG(LOADADDR(.rtos.data));
    LONG(_dram0_data_start);
    LONG(_dram0_data_end);
    LONG(LOADADDR(.dram0.data));
    LONG(_data_start);
    LONG(_data_end);
    LONG(LOADADDR(.data));
    LONG(_WindowVectors_text_start);
    LONG(_WindowVectors_text_end);
    LONG(LOADADDR(.WindowVectors.text));
    LONG(_Level2InterruptVector_literal_start);
    LONG(_Level2InterruptVector_literal_end);
    LONG(LOADADDR(.Level2InterruptVector.literal));
    LONG(_Level2InterruptVector_text_start);
    LONG(_Level2InterruptVector_text_end);
    LONG(LOADADDR(.Level2InterruptVector.text));
    LONG(_Level3InterruptVector_literal_start);
    LONG(_Level3InterruptVector_literal_end);
    LONG(LOADADDR(.Level3InterruptVector.literal));
    LONG(_Level3InterruptVector_text_start);
    LONG(_Level3InterruptVector_text_end);
    LONG(LOADADDR(.Level3InterruptVector.text));
    LONG(_Level4InterruptVector_literal_start);
    LONG(_Level4InterruptVector_literal_end);
    LONG(LOADADDR(.Level4InterruptVector.literal));
    LONG(_Level4InterruptVector_text_start);
    LONG(_Level4InterruptVector_text_end);
    LONG(LOADADDR(.Level4InterruptVector.text));
    LONG(_DebugExceptionVector_literal_start);
    LONG(_DebugExceptionVector_literal_end);
    LONG(LOADADDR(.DebugExceptionVector.literal));
    LONG(_DebugExceptionVector_text_start);
    LONG(_DebugExceptionVector_text_end);
    LONG(LOADADDR(.DebugExceptionVector.text));
    LONG(_KernelExceptionVector_literal_start);
    LONG(_KernelExceptionVector_literal_end);
    LONG(LOADADDR(.KernelExceptionVector.literal));
    LONG(_KernelExceptionVector_text_start);
    LONG(_KernelExceptionVector_text_end);
    LONG(LOADADDR(.KernelExceptionVector.text));
    LONG(_UserExceptionVector_literal_start);
    LONG(_UserExceptionVector_literal_end);
    LONG(LOADADDR(.UserExceptionVector.literal));
    LONG(_UserExceptionVector_text_start);
    LONG(_UserExceptionVector_text_end);
    LONG(LOADADDR(.UserExceptionVector.text));
    LONG(_DoubleExceptionVector_literal_start);
    LONG(_DoubleExceptionVector_literal_end);
    LONG(LOADADDR(.DoubleExceptionVector.literal));
    LONG(_DoubleExceptionVector_text_start);
    LONG(_DoubleExceptionVector_text_end);
    LONG(LOADADDR(.DoubleExceptionVector.text));
    LONG(_iram0_text_start);
    LONG(_iram0_text_end);
    LONG(LOADADDR(.iram0.text));
    LONG(_text_start);
    LONG(_text_end);
    LONG(LOADADDR(.text));
    LONG(_clib_text_start);
    LONG(_clib_text_end);
    LONG(LOADADDR(.clib.text));
    LONG(_rtos_text_start);
    LONG(_rtos_text_end);
    LONG(LOADADDR(.rtos.text));
    LONG(_sram_text_start);
    LONG(_sram_text_end);
    LONG(LOADADDR(.sram.text));
    LONG(0);
    LONG(0);
    LONG(0);
    _rom_store = .;
    . = ALIGN (4);
    _ResetHandler_text_end = ABSOLUTE(.);
    _memmap_seg_sram1_end = ALIGN(0x8);
  } >sram1_seg :sram1_phdr


  .srom.rodata : ALIGN(4)
  {
    _srom_rodata_start = ABSOLUTE(.);
    *(.srom.rodata)
    . = ALIGN (4);
    _srom_rodata_end = ABSOLUTE(.);
  } >srom0_seg :srom0_phdr

  .srom.text : ALIGN(4)
  {
    _srom_text_start = ABSOLUTE(.);
    *(.srom.literal .srom.text)
    . = ALIGN (4);
    _srom_text_end = ABSOLUTE(.);
    _memmap_seg_srom0_end = ALIGN(0x8);
  } >srom0_seg :srom0_phdr


  .sram.rodata : AT(LOADADDR(.ResetHandler.text) + ((SIZEOF(.ResetHandler.text) + 3) & 0xFFFFFFFC)) ALIGN(4)
  {
    _sram_rodata_start = ABSOLUTE(.);
    *(.sram.rodata)
    . = ALIGN (4);
    _sram_rodata_end = ABSOLUTE(.);
  } >dram0_0_seg :dram0_0_phdr

  .sram.data : AT(LOADADDR(.sram.rodata) + (ADDR(.sram.data) - ADDR(.sram.rodata))) ALIGN(4)
  {
    _sram_data_start = ABSOLUTE(.);
    *(.sram.data)
    . = ALIGN (4);
    _sram_data_end = ABSOLUTE(.);
  } >dram0_0_seg :dram0_0_phdr

  __llvm_prf_names : AT(LOADADDR(.sram.data) + (ADDR(__llvm_prf_names) - ADDR(.sram.data))) ALIGN(4)
  {
    __llvm_prf_names_start = ABSOLUTE(.);
    *(__llvm_prf_names)
    . = ALIGN (4);
    __llvm_prf_names_end = ABSOLUTE(.);
  } >dram0_0_seg :dram0_0_phdr

  .dram0.rodata : AT(LOADADDR(__llvm_prf_names) + (ADDR(.dram0.rodata) - ADDR(__llvm_prf_names))) ALIGN(4)
  {
    _dram0_rodata_start = ABSOLUTE(.);
    *(.dram0.rodata)
    *(.dram.rodata)
    . = ALIGN (4);
    _dram0_rodata_end = ABSOLUTE(.);
  } >dram0_0_seg :dram0_0_phdr

  .clib.rodata : AT(LOADADDR(.dram0.rodata) + (ADDR(.clib.rodata) - ADDR(.dram0.rodata))) ALIGN(4)
  {
    _clib_rodata_start = ABSOLUTE(.);
    *(.clib.rodata)
    . = ALIGN (4);
    _clib_rodata_end = ABSOLUTE(.);
  } >dram0_0_seg :dram0_0_phdr

  .rtos.rodata : AT(LOADADDR(.clib.rodata) + (ADDR(.rtos.rodata) - ADDR(.clib.rodata))) ALIGN(4)
  {
    _rtos_rodata_start = ABSOLUTE(.);
    *(.rtos.rodata)
    . = ALIGN (4);
    _rtos_rodata_end = ABSOLUTE(.);
  } >dram0_0_seg :dram0_0_phdr

  .rodata : AT(LOADADDR(.rtos.rodata) + (ADDR(.rodata) - ADDR(.rtos.rodata))) ALIGN(4)
  {
    _rodata_start = ABSOLUTE(.);
    *(.rodata)
    *(SORT(.rodata.sort.*))
    KEEP (*(SORT(.rodata.keepsort.*) .rodata.keep.*))
    *(.rodata.*)
    *(.gnu.linkonce.r.*)
    *(.rodata1)
    __XT_EXCEPTION_TABLE__ = ABSOLUTE(.);
    KEEP (*(.xt_except_table))
    KEEP (*(.gcc_except_table))
    *(.gnu.linkonce.e.*)
    *(.gnu.version_r)
    KEEP (*(.eh_frame))
    /*  C++ constructor and destructor tables, properly ordered:  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    KEEP (*crtbegin.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    /*  C++ exception handlers table:  */
    __XT_EXCEPTION_DESCS__ = ABSOLUTE(.);
    *(.xt_except_desc)
    *(.gnu.linkonce.h.*)
    __XT_EXCEPTION_DESCS_END__ = ABSOLUTE(.);
    *(.xt_except_desc_end)
    *(.dynamic)
    *(.gnu.version_d)
    . = ALIGN(4);		/* this table MUST be 4-byte aligned */
    _bss_table_start = ABSOLUTE(.);
    LONG(_bss_start)
    LONG(_bss_end)
    _bss_table_end = ABSOLUTE(.);
    . = ALIGN (4);
    _rodata_end = ABSOLUTE(.);
  } >dram0_0_seg :dram0_0_phdr

  .clib.data : AT(LOADADDR(.rodata) + (ADDR(.clib.data) - ADDR(.rodata))) ALIGN(4)
  {
    _clib_data_start = ABSOLUTE(.);
    *(.clib.data)
    . = ALIGN (4);
    _clib_data_end = ABSOLUTE(.);
  } >dram0_0_seg :dram0_0_phdr

  .clib.percpu.data : AT(LOADADDR(.clib.data) + (ADDR(.clib.percpu.data) - ADDR(.clib.data))) ALIGN(4)
  {
    _clib_percpu_data_start = ABSOLUTE(.);
    *(.clib.percpu.data)
    . = ALIGN (4);
    _clib_percpu_data_end = ABSOLUTE(.);
  } >dram0_0_seg :dram0_0_phdr

  .rtos.percpu.data : AT(LOADADDR(.clib.percpu.data) + (ADDR(.rtos.percpu.data) - ADDR(.clib.percpu.data))) ALIGN(4)
  {
    _rtos_percpu_data_start = ABSOLUTE(.);
    *(.rtos.percpu.data)
    . = ALIGN (4);
    _rtos_percpu_data_end = ABSOLUTE(.);
  } >dram0_0_seg :dram0_0_phdr

  .rtos.data : AT(LOADADDR(.rtos.percpu.data) + (ADDR(.rtos.data) - ADDR(.rtos.percpu.data))) ALIGN(4)
  {
    _rtos_data_start = ABSOLUTE(.);
    *(.rtos.data)
    . = ALIGN (4);
    _rtos_data_end = ABSOLUTE(.);
  } >dram0_0_seg :dram0_0_phdr

  .dram0.data : AT(LOADADDR(.rtos.data) + (ADDR(.dram0.data) - ADDR(.rtos.data))) ALIGN(4)
  {
    _dram0_data_start = ABSOLUTE(.);
    *(.dram0.data)
    *(.dram.data)
    . = ALIGN (4);
    _dram0_data_end = ABSOLUTE(.);
  } >dram0_0_seg :dram0_0_phdr

  .data : AT(LOADADDR(.dram0.data) + (ADDR(.data) - ADDR(.dram0.data))) ALIGN(4)
  {
    _data_start = ABSOLUTE(.);
    *(.data)
    *(SORT(.data.sort.*))
    KEEP (*(SORT(.data.keepsort.*) .data.keep.*))
    *(.data.*)
    *(.gnu.linkonce.d.*)
    KEEP(*(.gnu.linkonce.d.*personality*))
    *(.data1)
    *(.sdata)
    *(.sdata.*)
    *(.gnu.linkonce.s.*)
    *(.sdata2)
    *(.sdata2.*)
    *(.gnu.linkonce.s2.*)
    KEEP(*(.jcr))
    *(__llvm_prf_cnts)
    *(__llvm_prf_data)
    *(__llvm_prf_vnds)
    . = ALIGN (4);
    _data_end = ABSOLUTE(.);
  } >dram0_0_seg :dram0_0_phdr

  .bss (NOLOAD) : ALIGN(8)
  {
    . = ALIGN (8);
    _bss_start = ABSOLUTE(.);
    *(.dynsbss)
    *(.sbss)
    *(.sbss.*)
    *(.gnu.linkonce.sb.*)
    *(.scommon)
    *(.sbss2)
    *(.sbss2.*)
    *(.gnu.linkonce.sb2.*)
    *(.dynbss)
    *(.bss)
    *(SORT(.bss.sort.*))
    KEEP (*(SORT(.bss.keepsort.*) .bss.keep.*))
    *(.bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    *(.clib.bss)
    *(.clib.percpu.bss)
    *(.rtos.percpu.bss)
    *(.rtos.bss)
    *(.dram0.bss)
    *(.sram.bss)
    . = ALIGN (8);
    _bss_end = ABSOLUTE(.);
    _end = ALIGN(0x8);
    PROVIDE(end = ALIGN(0x8));
    _stack_sentry = ALIGN(0x8);
    _memmap_seg_dram0_0_end = ALIGN(0x8);
  } >dram0_0_seg :dram0_0_bss_phdr

  PROVIDE(__stack = 0x9ff80000);
  _heap_sentry = 0x9ff80000;

  .WindowVectors.text : AT(LOADADDR(.data) + ((SIZEOF(.data) + 3) & 0xFFFFFFFC)) ALIGN(4)
  {
    _WindowVectors_text_start = ABSOLUTE(.);
    KEEP (*(.WindowVectors.text))
    . = ALIGN (4);
    _WindowVectors_text_end = ABSOLUTE(.);
  } >iram0_0_seg :iram0_0_phdr

  .Level2InterruptVector.literal : AT(LOADADDR(.WindowVectors.text) + (ADDR(.Level2InterruptVector.literal) - ADDR(.WindowVectors.text))) ALIGN(4)
  {
    _Level2InterruptVector_literal_start = ABSOLUTE(.);
    *(.Level2InterruptVector.literal)
    . = ALIGN (4);
    _Level2InterruptVector_literal_end = ABSOLUTE(.);
    _memmap_seg_iram0_0_end = ALIGN(0x8);
  } >iram0_0_seg :iram0_0_phdr


  .Level2InterruptVector.text : AT(LOADADDR(.Level2InterruptVector.literal) + ((SIZEOF(.Level2InterruptVector.literal) + 3) & 0xFFFFFFFC)) ALIGN(4)
  {
    _Level2InterruptVector_text_start = ABSOLUTE(.);
    KEEP (*(.Level2InterruptVector.text))
    . = ALIGN (4);
    _Level2InterruptVector_text_end = ABSOLUTE(.);
  } >iram0_1_seg :iram0_1_phdr

  .Level3InterruptVector.literal : AT(LOADADDR(.Level2InterruptVector.text) + (ADDR(.Level3InterruptVector.literal) - ADDR(.Level2InterruptVector.text))) ALIGN(4)
  {
    _Level3InterruptVector_literal_start = ABSOLUTE(.);
    *(.Level3InterruptVector.literal)
    . = ALIGN (4);
    _Level3InterruptVector_literal_end = ABSOLUTE(.);
    _memmap_seg_iram0_1_end = ALIGN(0x8);
  } >iram0_1_seg :iram0_1_phdr


  .Level3InterruptVector.text : AT(LOADADDR(.Level3InterruptVector.literal) + ((SIZEOF(.Level3InterruptVector.literal) + 3) & 0xFFFFFFFC)) ALIGN(4)
  {
    _Level3InterruptVector_text_start = ABSOLUTE(.);
    KEEP (*(.Level3InterruptVector.text))
    . = ALIGN (4);
    _Level3InterruptVector_text_end = ABSOLUTE(.);
  } >iram0_2_seg :iram0_2_phdr

  .Level4InterruptVector.literal : AT(LOADADDR(.Level3InterruptVector.text) + (ADDR(.Level4InterruptVector.literal) - ADDR(.Level3InterruptVector.text))) ALIGN(4)
  {
    _Level4InterruptVector_literal_start = ABSOLUTE(.);
    *(.Level4InterruptVector.literal)
    . = ALIGN (4);
    _Level4InterruptVector_literal_end = ABSOLUTE(.);
    _memmap_seg_iram0_2_end = ALIGN(0x8);
  } >iram0_2_seg :iram0_2_phdr


  .Level4InterruptVector.text : AT(LOADADDR(.Level4InterruptVector.literal) + ((SIZEOF(.Level4InterruptVector.literal) + 3) & 0xFFFFFFFC)) ALIGN(4)
  {
    _Level4InterruptVector_text_start = ABSOLUTE(.);
    KEEP (*(.Level4InterruptVector.text))
    . = ALIGN (4);
    _Level4InterruptVector_text_end = ABSOLUTE(.);
  } >iram0_3_seg :iram0_3_phdr

  .DebugExceptionVector.literal : AT(LOADADDR(.Level4InterruptVector.text) + (ADDR(.DebugExceptionVector.literal) - ADDR(.Level4InterruptVector.text))) ALIGN(4)
  {
    _DebugExceptionVector_literal_start = ABSOLUTE(.);
    *(.DebugExceptionVector.literal)
    . = ALIGN (4);
    _DebugExceptionVector_literal_end = ABSOLUTE(.);
    _memmap_seg_iram0_3_end = ALIGN(0x8);
  } >iram0_3_seg :iram0_3_phdr


  .DebugExceptionVector.text : AT(LOADADDR(.DebugExceptionVector.literal) + ((SIZEOF(.DebugExceptionVector.literal) + 3) & 0xFFFFFFFC)) ALIGN(4)
  {
    _DebugExceptionVector_text_start = ABSOLUTE(.);
    KEEP (*(.DebugExceptionVector.text))
    . = ALIGN (4);
    _DebugExceptionVector_text_end = ABSOLUTE(.);
  } >iram0_4_seg :iram0_4_phdr

  .KernelExceptionVector.literal : AT(LOADADDR(.DebugExceptionVector.text) + (ADDR(.KernelExceptionVector.literal) - ADDR(.DebugExceptionVector.text))) ALIGN(4)
  {
    _KernelExceptionVector_literal_start = ABSOLUTE(.);
    *(.KernelExceptionVector.literal)
    . = ALIGN (4);
    _KernelExceptionVector_literal_end = ABSOLUTE(.);
    _memmap_seg_iram0_4_end = ALIGN(0x8);
  } >iram0_4_seg :iram0_4_phdr


  .KernelExceptionVector.text : AT(LOADADDR(.KernelExceptionVector.literal) + ((SIZEOF(.KernelExceptionVector.literal) + 3) & 0xFFFFFFFC)) ALIGN(4)
  {
    _KernelExceptionVector_text_start = ABSOLUTE(.);
    KEEP (*(.KernelExceptionVector.text))
    . = ALIGN (4);
    _KernelExceptionVector_text_end = ABSOLUTE(.);
  } >iram0_5_seg :iram0_5_phdr

  .UserExceptionVector.literal : AT(LOADADDR(.KernelExceptionVector.text) + (ADDR(.UserExceptionVector.literal) - ADDR(.KernelExceptionVector.text))) ALIGN(4)
  {
    _UserExceptionVector_literal_start = ABSOLUTE(.);
    *(.UserExceptionVector.literal)
    . = ALIGN (4);
    _UserExceptionVector_literal_end = ABSOLUTE(.);
    _memmap_seg_iram0_5_end = ALIGN(0x8);
  } >iram0_5_seg :iram0_5_phdr


  .UserExceptionVector.text : AT(LOADADDR(.UserExceptionVector.literal) + ((SIZEOF(.UserExceptionVector.literal) + 3) & 0xFFFFFFFC)) ALIGN(4)
  {
    _UserExceptionVector_text_start = ABSOLUTE(.);
    KEEP (*(.UserExceptionVector.text))
    . = ALIGN (4);
    _UserExceptionVector_text_end = ABSOLUTE(.);
  } >iram0_6_seg :iram0_6_phdr

  .DoubleExceptionVector.literal : AT(LOADADDR(.UserExceptionVector.text) + (ADDR(.DoubleExceptionVector.literal) - ADDR(.UserExceptionVector.text))) ALIGN(4)
  {
    _DoubleExceptionVector_literal_start = ABSOLUTE(.);
    *(.DoubleExceptionVector.literal)
    . = ALIGN (4);
    _DoubleExceptionVector_literal_end = ABSOLUTE(.);
    _memmap_seg_iram0_6_end = ALIGN(0x8);
  } >iram0_6_seg :iram0_6_phdr


  .DoubleExceptionVector.text : AT(LOADADDR(.DoubleExceptionVector.literal) + ((SIZEOF(.DoubleExceptionVector.literal) + 3) & 0xFFFFFFFC)) ALIGN(4)
  {
    _DoubleExceptionVector_text_start = ABSOLUTE(.);
    KEEP (*(.DoubleExceptionVector.text))
    . = ALIGN (4);
    _DoubleExceptionVector_text_end = ABSOLUTE(.);
  } >iram0_7_seg :iram0_7_phdr

  .iram0.text : AT(LOADADDR(.DoubleExceptionVector.text) + (ADDR(.iram0.text) - ADDR(.DoubleExceptionVector.text))) ALIGN(4)
  {
    _iram0_text_start = ABSOLUTE(.);
    *(.iram0.literal .iram.literal .iram.text.literal .iram0.text .iram.text)
    . = ALIGN (4);
    _iram0_text_end = ABSOLUTE(.);
  } >iram0_7_seg :iram0_7_phdr

  .text : AT(LOADADDR(.iram0.text) + (ADDR(.text) - ADDR(.iram0.text))) ALIGN(4)
  {
    _stext = .;
    _text_start = ABSOLUTE(.);
    *(.entry.text)
    *(.init.literal)
    KEEP(*(.init))
    *(.literal.sort.* SORT(.text.sort.*))
    KEEP (*(.literal.keepsort.* SORT(.text.keepsort.*) .literal.keep.* .text.keep.* .literal.*personality* .text.*personality*))
    *(.literal .text .literal.* .text.* .stub .gnu.warning .gnu.linkonce.literal.* .gnu.linkonce.t.*.literal .gnu.linkonce.t.*)
    *(.fini.literal)
    KEEP(*(.fini))
    *(.gnu.version)
    . = ALIGN (4);
    _text_end = ABSOLUTE(.);
    _etext = .;
  } >iram0_7_seg :iram0_7_phdr

  .clib.text : AT(LOADADDR(.text) + (ADDR(.clib.text) - ADDR(.text))) ALIGN(4)
  {
    _clib_text_start = ABSOLUTE(.);
    *(.clib.literal .clib.text)
    . = ALIGN (4);
    _clib_text_end = ABSOLUTE(.);
  } >iram0_7_seg :iram0_7_phdr

  .rtos.text : AT(LOADADDR(.clib.text) + (ADDR(.rtos.text) - ADDR(.clib.text))) ALIGN(4)
  {
    _rtos_text_start = ABSOLUTE(.);
    *(.rtos.literal .rtos.text)
    . = ALIGN (4);
    _rtos_text_end = ABSOLUTE(.);
  } >iram0_7_seg :iram0_7_phdr

  .sram.text : AT(LOADADDR(.rtos.text) + (ADDR(.sram.text) - ADDR(.rtos.text))) ALIGN(4)
  {
    _sram_text_start = ABSOLUTE(.);
    *(.sram.literal .sram.text)
    . = ALIGN (4);
    _sram_text_end = ABSOLUTE(.);
  } >iram0_7_seg :iram0_7_phdr


  .ResetVector.text : ALIGN(4)
  {
    _ResetVector_text_start = ABSOLUTE(.);
    KEEP (*(.ResetVector.text))
    . = ALIGN (4);
    _ResetVector_text_end = ABSOLUTE(.);
  } >sram0_seg :sram0_phdr

  .ResetHandler.literal : ALIGN(4)
  {
    _ResetHandler_literal_start = ABSOLUTE(.);
    *(.ResetHandler.literal)
    . = ALIGN (4);
    _ResetHandler_literal_end = ABSOLUTE(.);
    _memmap_seg_sram0_end = ALIGN(0x8);
  } >sram0_seg :sram0_phdr


  .rom_store_end LOADADDR(.sram.text) + SIZEOF(.sram.text) : 
  { 
    _rom_store_end = .;
  } >sram1_seg
  .debug  0 :  { *(.debug) }
  .line  0 :  { *(.line) }
  .debug_srcinfo  0 :  { *(.debug_srcinfo) }
  .debug_sfnames  0 :  { *(.debug_sfnames) }
  .debug_aranges  0 :  { *(.debug_aranges) }
  .debug_pubnames  0 :  { *(.debug_pubnames) }
  .debug_info  0 :  { *(.debug_info) }
  .debug_abbrev  0 :  { *(.debug_abbrev) }
  .debug_line  0 :  { *(.debug_line) }
  .debug_frame  0 :  { *(.debug_frame) }
  .debug_str  0 :  { *(.debug_str) }
  .debug_loc  0 :  { *(.debug_loc) }
  .debug_macinfo  0 :  { *(.debug_macinfo) }
  .debug_weaknames  0 :  { *(.debug_weaknames) }
  .debug_funcnames  0 :  { *(.debug_funcnames) }
  .debug_typenames  0 :  { *(.debug_typenames) }
  .debug_varnames  0 :  { *(.debug_varnames) }
  .xt.insn 0 :
  {
    KEEP (*(.xt.insn))
    KEEP (*(.gnu.linkonce.x.*))
  }
  .xt.prop 0 :
  {
    KEEP (*(.xt.prop))
    KEEP (*(.xt.prop.*))
    KEEP (*(.gnu.linkonce.prop.*))
  }
  .xt.lit 0 :
  {
    KEEP (*(.xt.lit))
    KEEP (*(.xt.lit.*))
    KEEP (*(.gnu.linkonce.p.*))
  }
  .debug.xt.callgraph 0 :
  {
    KEEP (*(.debug.xt.callgraph .debug.xt.callgraph.* .gnu.linkonce.xt.callgraph.*))
  }
}

