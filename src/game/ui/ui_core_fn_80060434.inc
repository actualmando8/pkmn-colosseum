    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r29, r3
    bl fn_8025DA88
    bl fn_8025D9A8
    mr r27, r3
    bl fn_8025DAD0
    lis r5, lbl_803A9A60@ha
    lis r4, lbl_802EF0A8@ha
    addi r5, r5, lbl_803A9A60@l
    lwz r0, 0x4(r5)
    addi r4, r4, lbl_802EF0A8@l
    lwz r31, 0x3c0(r5)
    addis r30, r4, 0x2
    cmpwi r0, 0x1
    beq @L_80060618
    bge @L_800608B0
    cmpwi r0, 0x0
    bge @L_8006048C
    b @L_800608B0
@L_8006048C:
    cmpwi r27, 0x2
    beq @L_800605D0
    bge @L_800604A8
    cmpwi r27, 0x0
    beq @L_800604B0
    bge @L_80060588
    b @L_800608B0
@L_800604A8:
    cmpwi r27, 0x4
    b @L_800608B0
@L_800604B0:
    cmpwi r31, 0x5
    bgt @L_80060500
    addi r4, r31, 0x1
    li r3, 0x2f
    bl fn_80132A38
    li r3, 0x3f39
    bl fn_800FA444
    lha r4, -0x2f92(r30)
    srwi r5, r3, 16
    lbz r3, 0x8b(r29)
    li r0, -0x100
    subf r7, r5, r4
    li r4, 0x0
    srwi r6, r7, 31
    or r5, r3, r0
    add r0, r6, r7
    li r6, 0x3f39
    srawi r3, r0, 1
    bl fn_800FB680
    b @L_800608B0
@L_80060500:
    cmpwi r31, 0x6
    bne @L_80060544
    li r3, 0x3f3a
    bl fn_800FA444
    lha r4, -0x2f92(r30)
    srwi r5, r3, 16
    lbz r3, 0x8b(r29)
    li r0, -0x100
    subf r7, r5, r4
    li r4, 0x0
    srwi r6, r7, 31
    or r5, r3, r0
    add r0, r6, r7
    li r6, 0x3f3a
    srawi r3, r0, 1
    bl fn_800FB680
    b @L_800608B0
@L_80060544:
    cmpwi r31, 0x7
    bne @L_800608B0
    li r3, 0x3f3b
    bl fn_800FA444
    lha r4, -0x2f92(r30)
    srwi r5, r3, 16
    lbz r3, 0x8b(r29)
    li r0, -0x100
    subf r7, r5, r4
    li r4, 0x0
    srwi r6, r7, 31
    or r5, r3, r0
    add r0, r6, r7
    li r6, 0x3f3b
    srawi r3, r0, 1
    bl fn_800FB680
    b @L_800608B0
@L_80060588:
    addi r4, r31, 0x1
    li r3, 0x2f
    bl fn_80132A38
    li r3, 0x3f3c
    bl fn_800FA444
    lha r4, -0x2f92(r30)
    srwi r5, r3, 16
    lbz r3, 0x8b(r29)
    li r0, -0x100
    subf r7, r5, r4
    li r4, 0x0
    srwi r6, r7, 31
    or r5, r3, r0
    add r0, r6, r7
    li r6, 0x3f3c
    srawi r3, r0, 1
    bl fn_800FB680
    b @L_800608B0
@L_800605D0:
    lis r4, lbl_802ED9A0@ha
    slwi r28, r3, 2
    addi r27, r4, lbl_802ED9A0@l
    lwzx r3, r27, r28
    bl fn_800FA444
    lha r4, -0x2f92(r30)
    srwi r5, r3, 16
    lbz r3, 0x8b(r29)
    li r0, -0x100
    subf r7, r5, r4
    lwzx r6, r27, r28
    srwi r4, r7, 31
    or r5, r3, r0
    add r0, r4, r7
    li r4, 0x0
    srawi r3, r0, 1
    bl fn_800FB680
    b @L_800608B0
@L_80060618:
    cmpwi r27, 0x2
    beq @L_8006082C
    bge @L_80060634
    cmpwi r27, 0x0
    beq @L_8006063C
    bge @L_800607B0
    b @L_800608B0
@L_80060634:
    cmpwi r27, 0x4
    b @L_800608B0
@L_8006063C:
    cmpwi r31, 0x5
    bgt @L_800606C0
    addi r4, r31, 0x1
    li r3, 0x2f
    bl fn_80132A38
    li r3, 0x3f39
    bl fn_800FA444
    srwi r4, r3, 16
    li r3, 0x3f3d
    addi r26, r4, 0xb
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, -0x2f92(r30)
    add r4, r26, r3
    lbz r3, 0x8b(r29)
    subf r6, r4, r0
    li r0, -0x100
    srwi r5, r6, 31
    li r4, 0x0
    add r6, r5, r6
    or r5, r3, r0
    srawi r27, r6, 1
    li r6, 0x3f39
    mr r3, r27
    bl fn_800FB680
    lbz r5, 0x8b(r29)
    li r0, -0x100
    add r3, r27, r26
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x3f3d
    bl fn_800FB680
    b @L_800608B0
@L_800606C0:
    cmpwi r31, 0x6
    bne @L_80060738
    li r3, 0x3f3a
    bl fn_800FA444
    srwi r4, r3, 16
    li r3, 0x3f3d
    addi r26, r4, 0x9
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, -0x2f92(r30)
    add r4, r26, r3
    lbz r3, 0x8b(r29)
    subf r6, r4, r0
    li r0, -0x100
    srwi r5, r6, 31
    li r4, 0x0
    add r6, r5, r6
    or r5, r3, r0
    srawi r27, r6, 1
    li r6, 0x3f3a
    mr r3, r27
    bl fn_800FB680
    lbz r5, 0x8b(r29)
    li r0, -0x100
    add r3, r27, r26
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x3f3d
    bl fn_800FB680
    b @L_800608B0
@L_80060738:
    cmpwi r31, 0x7
    bne @L_800608B0
    li r3, 0x3f3b
    bl fn_800FA444
    srwi r4, r3, 16
    li r3, 0x3f3d
    addi r26, r4, 0x9
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, -0x2f92(r30)
    add r4, r26, r3
    lbz r3, 0x8b(r29)
    subf r6, r4, r0
    li r0, -0x100
    srwi r5, r6, 31
    li r4, 0x0
    add r6, r5, r6
    or r5, r3, r0
    srawi r27, r6, 1
    li r6, 0x3f3b
    mr r3, r27
    bl fn_800FB680
    lbz r5, 0x8b(r29)
    li r0, -0x100
    add r3, r26, r27
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x3f3d
    bl fn_800FB680
    b @L_800608B0
@L_800607B0:
    li r3, 0x3f3c
    bl fn_800FA444
    srwi r4, r3, 16
    li r3, 0x3f3d
    addi r26, r4, 0xb
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, -0x2f92(r30)
    add r3, r26, r3
    addi r4, r31, 0x1
    subf r5, r3, r0
    li r3, 0x2f
    srwi r0, r5, 31
    add r0, r0, r5
    srawi r27, r0, 1
    bl fn_80132A38
    lbz r5, 0x8b(r29)
    li r0, -0x100
    mr r3, r27
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x3f3c
    bl fn_800FB680
    lbz r5, 0x8b(r29)
    li r0, -0x100
    add r3, r26, r27
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x3f3d
    bl fn_800FB680
    b @L_800608B0
@L_8006082C:
    lis r4, lbl_802ED9A0@ha
    slwi r27, r3, 2
    addi r28, r4, lbl_802ED9A0@l
    lwzx r3, r28, r27
    bl fn_800FA444
    srwi r4, r3, 16
    li r3, 0x3f3d
    addi r26, r4, 0xb
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, -0x2f92(r30)
    add r3, r26, r3
    mr r4, r31
    subf r5, r3, r0
    li r3, 0x2f
    srwi r0, r5, 31
    add r0, r0, r5
    srawi r30, r0, 1
    bl fn_80132A38
    lbz r4, 0x8b(r29)
    li r0, -0x100
    lwzx r6, r28, r27
    mr r3, r30
    or r5, r4, r0
    li r4, 0x0
    bl fn_800FB680
    lbz r5, 0x8b(r29)
    li r0, -0x100
    add r3, r26, r30
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x3f3d
    bl fn_800FB680
@L_800608B0:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
