    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r29, r4
    mr r28, r3
    mr r3, r29
    bl fn_80206608
    clrlwi. r0, r3, 24
    bne @L_801F8C30
    li r3, 0x1
    b @L_801F8D6C
    @L_801F8C30:
    cmplwi r28, 0x0
    bne @L_801F8C40
    li r30, 0x0
    b @L_801F8CB8
    @L_801F8C40:
    cmplwi r29, 0x0
    bne @L_801F8C50
    li r30, 0x0
    b @L_801F8CB8
    @L_801F8C50:
    li r31, 0x0
    b @L_801F8CA8
    @L_801F8C58:
    mr r3, r28
    mr r6, r31
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr r30, r3
    bl fn_80206780
    clrlwi. r0, r3, 24
    bne @L_801F8C80
    li r30, 0x0
    @L_801F8C80:
    cmplwi r30, 0x0
    beq @L_801F8CA4
    mr r3, r29
    mr r4, r30
    bl fn_80204928
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F8CA4
    b @L_801F8CB8
    @L_801F8CA4:
    addi r31, r31, 0x1
    @L_801F8CA8:
    clrlwi r0, r31, 16
    cmplwi r0, 0x2
    blt @L_801F8C58
    li r30, 0x0
    @L_801F8CB8:
    cmplwi r30, 0x0
    beq @L_801F8CC8
    li r3, 0x2
    b @L_801F8D6C
    @L_801F8CC8:
    cmplwi r28, 0x0
    bne @L_801F8CD8
    li r0, 0x0
    b @L_801F8D54
    @L_801F8CD8:
    cmplwi r29, 0x0
    bne @L_801F8CE8
    li r0, 0x0
    b @L_801F8D54
    @L_801F8CE8:
    li r31, 0x0
    b @L_801F8D44
    @L_801F8CF0:
    mr r3, r28
    mr r6, r31
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr r30, r3
    bl fn_80206780
    clrlwi. r0, r3, 24
    bne @L_801F8D18
    li r30, 0x0
    @L_801F8D18:
    cmplwi r30, 0x0
    beq @L_801F8D40
    mr r3, r30
    mr r4, r29
    bl fn_80204854
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F8D40
    li r0, 0x1
    b @L_801F8D54
    @L_801F8D40:
    addi r31, r31, 0x1
    @L_801F8D44:
    clrlwi r0, r31, 16
    cmplwi r0, 0x2
    blt @L_801F8CF0
    li r0, 0x0
    @L_801F8D54:
    clrlwi r3, r0, 24
    li r0, 0x3
    subi r3, r3, 0x1
    subic r3, r3, 0x1
    subfe r3, r3, r3
    and r3, r0, r3
    @L_801F8D6C:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
