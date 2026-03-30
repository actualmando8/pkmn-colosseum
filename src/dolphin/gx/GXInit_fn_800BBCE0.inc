    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    rlwinm r0, r5, 0, 16, 31
    cmpwi r0, 0x40
    stwu r1, -0x70(r1)
    stmw r24, 0x50(r1)
    addi r24, r3, 0x0
    lwz r28, 0x78(r1)
    addi r25, r4, 0x0
    lwz r29, 0x7c(r1)
    addi r26, r9, 0x0
    addi r27, r10, 0x0
    beq @L_800BBD58
    bge @L_800BBD30
    cmpwi r0, 0x20
    beq @L_800BBD60
    bge @L_800BBD70
    cmpwi r0, 0x10
    beq @L_800BBD68
    b @L_800BBD70
@L_800BBD30:
    cmpwi r0, 0x100
    beq @L_800BBD48
    bge @L_800BBD70
    cmpwi r0, 0x80
    beq @L_800BBD50
    b @L_800BBD70
@L_800BBD48:
    li r31, 0x1
    b @L_800BBD74
@L_800BBD50:
    li r31, 0x2
    b @L_800BBD74
@L_800BBD58:
    li r31, 0x3
    b @L_800BBD74
@L_800BBD60:
    li r31, 0x4
    b @L_800BBD74
@L_800BBD68:
    li r31, 0x5
    b @L_800BBD74
@L_800BBD70:
    li r31, 0x0
@L_800BBD74:
    rlwinm r0, r6, 0, 16, 31
    cmpwi r0, 0x40
    beq @L_800BBDC4
    bge @L_800BBD9C
    cmpwi r0, 0x20
    beq @L_800BBDCC
    bge @L_800BBDDC
    cmpwi r0, 0x10
    beq @L_800BBDD4
    b @L_800BBDDC
@L_800BBD9C:
    cmpwi r0, 0x100
    beq @L_800BBDB4
    bge @L_800BBDDC
    cmpwi r0, 0x80
    beq @L_800BBDBC
    b @L_800BBDDC
@L_800BBDB4:
    li r30, 0x1
    b @L_800BBDE0
@L_800BBDBC:
    li r30, 0x2
    b @L_800BBDE0
@L_800BBDC4:
    li r30, 0x3
    b @L_800BBDE0
@L_800BBDCC:
    li r30, 0x4
    b @L_800BBDE0
@L_800BBDD4:
    li r30, 0x5
    b @L_800BBDE0
@L_800BBDDC:
    li r30, 0x0
@L_800BBDE0:
    rlwinm r0, r7, 0, 16, 31
    lfd f4, -29472(r2)
    stw r0, 0x4c(r1)
    lis r6, 0x4330
    rlwinm r0, r8, 0, 16, 31
    lfs f3, -29484(r2)
    stw r6, 0x48(r1)
    lfs f1, -29480(r2)
    mr r3, r27
    lfd f0, 0x48(r1)
    addi r4, r1, 0x28
    stw r0, 0x44(r1)
    fsubs f0, f0, f4
    li r5, 0xa
    stw r6, 0x40(r1)
    fmuls f2, f0, f3
    lfd f0, 0x40(r1)
    fsubs f0, f0, f4
    stfs f2, 0x28(r1)
    stfs f1, 0x30(r1)
    fmuls f0, f0, f3
    stfs f1, 0x2c(r1)
    stfs f0, 0x38(r1)
    stfs f1, 0x3c(r1)
    stfs f1, 0x34(r1)
    bl fn_800BB81C
    li r0, 0x1
    stw r0, 0x8(r1)
    addi r3, r24, 0x0
    addi r4, r25, 0x0
    stw r29, 0xc(r1)
    addi r5, r26, 0x0
    addi r6, r28, 0x0
    addi r7, r27, 0x0
    addi r8, r31, 0x0
    addi r9, r30, 0x0
    li r10, 0x0
    bl fn_800BB780
    lmw r24, 0x50(r1)
    lwz r0, 0x74(r1)
    addi r1, r1, 0x70
    mtlr r0
    blr
