    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x2
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    bl fn_80129280
    mr. r29, r3
    beq @L_801ED4A4
    li r30, 0x0
    b @L_801ED498
@L_801ED3E4:
    mr r3, r29
    clrlwi r4, r30, 16
    bl fn_8012AC08
    mr r28, r3
    bl fn_80123FBC
    clrlwi. r0, r3, 24
    beq @L_801ED494
    cmplwi r28, 0x0
    beq @L_801ED494
    mr r3, r28
    bl fn_8011EE40
    clrlwi. r0, r3, 16
    mr r27, r3
    beq @L_801ED42C
    bl fn_801EEC74
    clrlwi. r0, r3, 24
    beq @L_801ED42C
    li r27, 0x0
@L_801ED42C:
    clrlwi. r0, r27, 16
    beq @L_801ED494
    mr r3, r28
    bl fn_8011EE28
    cmplwi r3, 0x0
    beq @L_801ED494
    mr r3, r27
    bl fn_801EE470
    clrlwi r0, r3, 16
    cmplwi r0, 0x100
    blt @L_801ED478
    mr r3, r28
    li r4, 0x0
    li r5, 0x1
    bl fn_8011F910
    mr r3, r27
    li r4, 0x0
    bl fn_801EE4DC
    b @L_801ED494
@L_801ED478:
    mr r3, r27
    bl fn_801EE470
    mr r4, r3
    mr r3, r27
    addi r0, r4, 0x1
    clrlwi r4, r0, 16
    bl fn_801EE4DC
@L_801ED494:
    addi r30, r30, 0x1
@L_801ED498:
    clrlwi r0, r30, 16
    cmplwi r0, 0x6
    blt @L_801ED3E4
@L_801ED4A4:
    li r3, 0xd0
    bl fn_801906A0
    cmplwi r3, 0x0
    beq @L_801ED4DC
    li r3, 0xd1
    bl fn_801906A0
    clrlwi r31, r3, 16
    cmplwi r31, 0x2710
    bge @L_801ED4D0
    addi r0, r31, 0x1
    clrlwi r31, r0, 16
@L_801ED4D0:
    clrlwi r4, r31, 16
    li r3, 0xd1
    bl fn_8019075C
@L_801ED4DC:
    li r3, 0x0
    li r4, 0xb
    bl fn_80129280
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801ED62C
    li r3, 0x0
    li r4, 0xb
    bl fn_80129280
    addic. r27, r3, 0x8
    li r28, 0x0
    beq @L_801ED54C
    mr r3, r27
    bl fn_8011EE40
    clrlwi. r0, r3, 16
    mr r28, r3
    beq @L_801ED530
    bl fn_801EEC74
    clrlwi. r0, r3, 24
    beq @L_801ED530
    li r28, 0x0
@L_801ED530:
    clrlwi. r0, r28, 16
    beq @L_801ED54C
    mr r3, r28
    bl fn_801EEC74
    clrlwi. r0, r3, 24
    beq @L_801ED54C
    li r28, 0x0
@L_801ED54C:
    clrlwi. r0, r28, 16
    beq @L_801ED5B8
    mr r3, r27
    bl fn_8011EE28
    cmplwi r3, 0x0
    beq @L_801ED614
    mr r3, r28
    bl fn_801EE470
    clrlwi r0, r3, 16
    cmplwi r0, 0x100
    blt @L_801ED598
    mr r3, r27
    li r4, 0x0
    li r5, 0x3
    bl fn_8011F910
    mr r3, r28
    li r4, 0x0
    bl fn_801EE4DC
    b @L_801ED614
@L_801ED598:
    mr r3, r28
    bl fn_801EE470
    mr r4, r3
    mr r3, r28
    addi r0, r4, 0x1
    clrlwi r4, r0, 16
    bl fn_801EE4DC
    b @L_801ED614
@L_801ED5B8:
    mr r3, r27
    bl fn_8011F4A8
    clrlwi r0, r3, 24
    cmplwi r0, 0x64
    bge @L_801ED614
    mr r3, r27
    bl fn_8011F4C0
    li r0, -0x1
    mr r4, r3
    cmplw r4, r0
    bge @L_801ED5E8
    addi r4, r4, 0x1
@L_801ED5E8:
    mr r3, r27
    bl fn_80125424
    li r0, 0x0
    mr r3, r27
    stb r0, 0x8(r1)
    bl fn_8011F4A8
    mr r4, r3
    mr r3, r27
    addi r6, r1, 0x8
    li r5, 0x1
    bl fn_8012361C
@L_801ED614:
    li r3, 0x0
    li r4, 0xb
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_801ED62C
    sth r31, 0x2(r3)
@L_801ED62C:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
