    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, lbl_80466BC0@ha
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    addi r3, r4, lbl_80466BC0@l
    lwz r0, 0x1d4(r3)
    cmpwi r0, 0x2
    blt @L_801BFA04
    lis r3, lbl_80466BC0@ha
    li r31, -0x1
    addi r3, r3, lbl_80466BC0@l
    lwz r0, 0x1d4(r3)
    cmpwi r0, 0x2
    blt @L_801BF9AC
    b @L_801BF8EC
    @L_801BF8E8:
    bl fn_800A8FE4
    @L_801BF8EC:
    lis r3, lbl_80466BC0@ha
    li r31, -0x1
    addi r3, r3, lbl_80466BC0@l
    lwz r0, 0x1d4(r3)
    cmpwi r0, 0x2
    blt @L_801BF9A4
    bl OSDisableInterrupts
    li r31, 0x0
    b @L_801BF934
    @L_801BF910:
    mulli r5, r31, 0x60
    lis r4, lbl_80466BC0@ha
    addi r0, r4, lbl_80466BC0@l
    add r4, r0, r5
    lwz r0, 0x5c(r4)
    cmpwi r0, 0x3
    bne @L_801BF930
    b @L_801BF940
    @L_801BF930:
    addi r31, r31, 0x1
    @L_801BF934:
    cmpwi r31, 0x3
    blt @L_801BF910
    li r31, -0x1
    @L_801BF940:
    cmpwi r31, -0x1
    bne @L_801BF9A0
    li r31, 0x0
    b @L_801BF974
    @L_801BF950:
    mulli r5, r31, 0x60
    lis r4, lbl_80466BC0@ha
    addi r0, r4, lbl_80466BC0@l
    add r4, r0, r5
    lwz r0, 0x5c(r4)
    cmpwi r0, 0x2
    bne @L_801BF970
    b @L_801BF980
    @L_801BF970:
    addi r31, r31, 0x1
    @L_801BF974:
    cmpwi r31, 0x3
    blt @L_801BF950
    li r31, -0x1
    @L_801BF980:
    cmpwi r31, -0x1
    beq @L_801BF9A0
    mulli r5, r31, 0x60
    lis r4, lbl_80466BC0@ha
    li r6, 0x3
    addi r0, r4, lbl_80466BC0@l
    add r4, r0, r5
    stw r6, 0x5c(r4)
    @L_801BF9A0:
    bl OSRestoreInterrupts
    @L_801BF9A4:
    cmpwi r31, -0x1
    beq @L_801BF8E8
    @L_801BF9AC:
    mulli r0, r31, 0x60
    lis r4, lbl_80466BC0@ha
    lis r3, lbl_80466BC0@ha
    addi r4, r4, lbl_80466BC0@l
    add r4, r4, r0
    addi r3, r3, lbl_80466BC0@l
    lwz r4, 0x58(r4)
    mr r5, r30
    bl fn_801BFA1C
    b @L_801BF9D8
    @L_801BF9D4:
    bl fn_800B8DA8
    @L_801BF9D8:
    bl fn_801BFCA0
    cmpwi r3, 0x0
    bne @L_801BF9D4
    lis r4, lbl_80466BC0@ha
    lis r3, lbl_80466BC0@ha
    addi r3, r3, lbl_80466BC0@l
    li r0, 0x1
    addi r4, r4, lbl_80466BC0@l
    stw r31, 0x1e4(r3)
    stw r0, 0x1e0(r4)
    bl fn_800B8DF4
    @L_801BFA04:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
