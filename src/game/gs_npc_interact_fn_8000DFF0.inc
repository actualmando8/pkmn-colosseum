    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r31, r3
    bl fn_80105624
    mr r30, r3
    mr r3, r31
    li r29, -0x1
    li r4, 0x0
    bl fn_801040D0
    lhz r4, 0x4(r30)
    clrlwi r0, r4, 31
    cmpwi r0, 0x0
    beq @L_8000E03C
    li r29, 0x0
    b @L_8000E084
    @L_8000E03C:
    rlwinm r0, r4, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_8000E050
    li r29, 0x1
    b @L_8000E084
    @L_8000E050:
    rlwinm r0, r4, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_8000E064
    li r29, 0x2
    b @L_8000E084
    @L_8000E064:
    rlwinm r0, r4, 0, 22, 22
    cmpwi r0, 0x0
    beq @L_8000E084
    li r4, 0x1
    li r0, -0x1
    stb r4, 0x98(r31)
    stb r4, 0x99(r31)
    stw r0, 0x80(r31)
    @L_8000E084:
    cmpwi r29, 0x0
    blt @L_8000E174
    li r4, 0x0
    li r0, 0x2
    mtctr r0
    @L_8000E098:
    lwz r0, 0x4(r3)
    cmpwi r0, 0x47
    beq @L_8000E0D4
    bge @L_8000E0B8
    cmpwi r0, 0x45
    beq @L_8000E0C4
    bge @L_8000E0CC
    b @L_8000E0E4
    @L_8000E0B8:
    cmpwi r0, 0x49
    bge @L_8000E0E4
    b @L_8000E0DC
    @L_8000E0C4:
    li r0, 0x1
    b @L_8000E0E8
    @L_8000E0CC:
    li r0, 0x0
    b @L_8000E0E8
    @L_8000E0D4:
    li r0, 0x2
    b @L_8000E0E8
    @L_8000E0DC:
    li r0, 0x2
    b @L_8000E0E8
    @L_8000E0E4:
    li r0, -0x1
    @L_8000E0E8:
    cmpw r29, r0
    bne @L_8000E100
    li r0, 0x1
    stb r0, 0x98(r31)
    stw r29, 0x80(r31)
    b @L_8000E1E8
    @L_8000E100:
    lwz r0, 0xc(r3)
    cmpwi r0, 0x47
    beq @L_8000E13C
    bge @L_8000E120
    cmpwi r0, 0x45
    beq @L_8000E12C
    bge @L_8000E134
    b @L_8000E14C
    @L_8000E120:
    cmpwi r0, 0x49
    bge @L_8000E14C
    b @L_8000E144
    @L_8000E12C:
    li r0, 0x1
    b @L_8000E150
    @L_8000E134:
    li r0, 0x0
    b @L_8000E150
    @L_8000E13C:
    li r0, 0x2
    b @L_8000E150
    @L_8000E144:
    li r0, 0x2
    b @L_8000E150
    @L_8000E14C:
    li r0, -0x1
    @L_8000E150:
    cmpw r29, r0
    bne @L_8000E168
    li r0, 0x1
    stb r0, 0x98(r31)
    stw r29, 0x80(r31)
    b @L_8000E1E8
    @L_8000E168:
    addi r3, r3, 0x10
    addi r4, r4, 0x1
    bdnz @L_8000E098
    @L_8000E174:
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8000E1CC
    li r3, 0x0
    bl fn_801F1700
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8000E1B4
    bl fn_80265924
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8000E1B4
    li r0, 0x1
    b @L_8000E1D0
    @L_8000E1B4:
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_8000E1CC
    li r0, 0x1
    b @L_8000E1D0
    @L_8000E1CC:
    li r0, 0x0
    @L_8000E1D0:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    beq @L_8000E1E8
    li r0, 0x1
    stb r0, 0x98(r31)
    stb r0, 0x99(r31)
    @L_8000E1E8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
