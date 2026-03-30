    nofralloc
    mflr r0
    cmpwi r4, 0x0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    addi r31, r3, 0x0
    mulli r5, r31, 0x110
    stw r30, 0x10(r1)
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    add r30, r0, r5
    blt @L_800AFB40
    lis r3, fn_800AFACC@ha
    addi r0, r3, fn_800AFACC@l
    lis r3, fn_800AF51C@ha
    stw r0, 0xdc(r30)
    addi r5, r3, fn_800AF51C@l
    addi r3, r31, 0x0
    li r4, 0x0
    bl EXILock
    cmpwi r3, 0x0
    bne @L_800AFB2C
    li r4, 0x0
    b @L_800AFB40
    @L_800AFB2C:
    li r0, 0x0
    stw r0, 0xdc(r30)
    mr r3, r31
    bl fn_800AF8A0
    mr r4, r3
    @L_800AFB40:
    cmpwi r4, 0x0
    bge @L_800AFBC4
    lbz r0, 0x94(r30)
    cmpwi r0, 0xf3
    beq @L_800AFBC4
    bge @L_800AFB70
    cmpwi r0, 0x52
    beq @L_800AFB7C
    blt @L_800AFBC4
    cmpwi r0, 0xf1
    bge @L_800AFBA4
    b @L_800AFBC4
    @L_800AFB70:
    cmpwi r0, 0xf5
    bge @L_800AFBC4
    b @L_800AFBA4
    @L_800AFB7C:
    lwz r0, 0xc8(r30)
    cmplwi r0, 0x0
    mr r12, r0
    beq @L_800AFBC4
    li r0, 0x0
    mtlr r12
    stw r0, 0xc8(r30)
    mr r3, r31
    blrl
    b @L_800AFBC4
    @L_800AFBA4:
    lwz r12, 0xcc(r30)
    cmplwi r12, 0x0
    beq @L_800AFBC4
    li r0, 0x0
    mtlr r12
    stw r0, 0xcc(r30)
    mr r3, r31
    blrl
    @L_800AFBC4:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
