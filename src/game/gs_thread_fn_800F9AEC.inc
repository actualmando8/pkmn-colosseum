    nofralloc
    cmpwi r5, 0x7
    beq @L_800F9B88
    bge @L_800F9B04
    cmpwi r5, 0x1
    beq @L_800F9B0C
    b @L_800F9B88
@L_800F9B04:
    cmpwi r5, 0x9
    b @L_800F9B88
@L_800F9B0C:
    cmplwi r4, 0x0
    li r7, 0x0
    bne @L_800F9B1C
    b @L_800F9B80
@L_800F9B1C:
    lis r5, lbl_80271300@ha
    addi r5, r5, lbl_80271300@l
    b @L_800F9B74
@L_800F9B28:
    mr r8, r5
    li r6, 0x0
    b @L_800F9B4C
@L_800F9B34:
    addi r6, r6, 0x1
    addi r8, r8, 0x2
    cmpwi r6, 0x100
    blt @L_800F9B4C
    li r6, 0xb7
    b @L_800F9B58
@L_800F9B4C:
    lhz r0, 0x0(r8)
    cmplw r9, r0
    bne @L_800F9B34
@L_800F9B58:
    cmplwi r3, 0x0
    beq @L_800F9B6C
    clrlwi r0, r6, 24
    stb r0, 0x0(r3)
    addi r3, r3, 0x1
@L_800F9B6C:
    addi r7, r7, 0x1
    addi r4, r4, 0x2
@L_800F9B74:
    lhz r9, 0x0(r4)
    cmplwi r9, 0x0
    bne @L_800F9B28
@L_800F9B80:
    mr r3, r7
    blr
@L_800F9B88:
    cmplwi r4, 0x0
    li r7, 0x0
    bne @L_800F9B98
    b @L_800F9BFC
@L_800F9B98:
    lis r5, lbl_80271500@ha
    addi r5, r5, lbl_80271500@l
    b @L_800F9BF0
@L_800F9BA4:
    mr r8, r5
    li r6, 0x0
    b @L_800F9BC8
@L_800F9BB0:
    addi r6, r6, 0x1
    addi r8, r8, 0x2
    cmpwi r6, 0x100
    blt @L_800F9BC8
    li r6, 0xb7
    b @L_800F9BD4
@L_800F9BC8:
    lhz r0, 0x0(r8)
    cmplw r9, r0
    bne @L_800F9BB0
@L_800F9BD4:
    cmplwi r3, 0x0
    beq @L_800F9BE8
    clrlwi r0, r6, 24
    stb r0, 0x0(r3)
    addi r3, r3, 0x1
@L_800F9BE8:
    addi r7, r7, 0x1
    addi r4, r4, 0x2
@L_800F9BF0:
    lhz r9, 0x0(r4)
    cmplwi r9, 0x0
    bne @L_800F9BA4
@L_800F9BFC:
    mr r3, r7
    blr
