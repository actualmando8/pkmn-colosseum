    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lbz r0, -21472(r13)
    cmplwi r0, 0x0
    beq @L_801E1718
    lbz r0, -21471(r13)
    cmplwi r0, 0x0
    bne @L_801E1720
@L_801E1718:
    li r0, 0x0
    b @L_801E1724
@L_801E1720:
    li r0, 0x1
@L_801E1724:
    clrlwi. r0, r0, 24
    beq @L_801E1794
    bl fn_801E386C
    bl fn_801E38D8
    cmpwi r3, 0x4
    beq @L_801E1794
    bge @L_801E174C
    cmpwi r3, 0x3
    bge @L_801E1754
    b @L_801E1794
@L_801E174C:
    cmpwi r3, 0x6
    bge @L_801E1794
@L_801E1754:
    lbz r0, -21471(r13)
    cmplwi r0, 0x0
    beq @L_801E1794
    bl fn_801E3F54
    bl fn_801E4724
    lwz r3, -21456(r13)
    bl fn_800E202C
    clrlwi. r0, r3, 16
    mr r31, r3
    beq @L_801E1788
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
@L_801E1788:
    li r0, 0x0
    stb r0, -21471(r13)
    bl fn_800EE928
@L_801E1794:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
